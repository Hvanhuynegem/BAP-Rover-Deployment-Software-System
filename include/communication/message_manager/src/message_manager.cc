#include "./message_manager.h"
#include <arpa/inet.h>    // Include the IP address conversion header
#include <cerrno>         // Include the errno header
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <mutex>
#include <netinet/in.h>    // Include the internet address header
#include <pthread.h>
#include <sys/socket.h>    // Include the socket header
#include <unistd.h>        // Include the header for close()

//TODO: Remove printf comments and implement logging

#define MAX_BUFFERS 5

using namespace std;

pthread_t receiving_thread;

// TODO: what is this used for?
Message_t source_message;

Message_t received_message;

/* Function used by `receiving_thread` to receive sent packets and
 * handle them or store them into `received_packet` */
static void *receive_message(void *data)
{
    // Cast the data parameter into Message_manager
    Message_manager_t *manager = static_cast<Message_manager_t *>(data);

    if(manager->get_receive_handler() == nullptr)
        return NULL;

    // loop while connected
    while(manager->compare_state(MSG_M_ST_CONN))
    {
        // Wait (blocking) until a packet is received
        Message_t packet;
        Message_manager_error_t err = manager->wait_and_receive_message(packet);
        if(err)
        {
            // TODO: notify error
            //cout << "err " << err << " in r_pack recv_m thread" << endl;
            continue;
        }

        // get the handler, and pass in the packets to be handled
        receive_handler handler = manager->get_receive_handler();
        if(handler != nullptr)
        {
            handler(&packet, &source_message);
        }
    }

    //cout << "" << endl;
    return NULL;
}

// Not used
// void Message_manager::subsr_isr(void(*func)(uint8_t,uint8_t,void*))
// {
//     this->isr_func = func;
// }

/* Sets up the file descriptor set and waits (blocking) until a message is sent
 */
void Message_manager_t::await_new_messsage()
{
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(socket_fd, &fdset);
    select(socket_fd + 1, &fdset, NULL, NULL, NULL);
}

/* Compares the argument state with the current state of the object. Returns
 * true if equal */
bool Message_manager_t::compare_state(Message_manager_state_t s)
{
    return s == state;
}

/* Creates a socket and connects to the port given in the argument. Then sends
 * an alive message to establish communication and starts the receiving_thread
 */
Message_manager_error_t Message_manager_t::connect_to_port(uint16_t port)
{
    // Check if the connection is already established

    if(compare_state(MSG_M_ST_CONN))
    {
        //printf("already connected\n");
        return MSG_M_ERROR_NONE;
    }

    state = MSG_M_ST_INIT;

    //printf("connecting to port %d\n", port);

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1)
    {
        //printf("socket failed\n");
        state = MSG_M_ST_ERR;
        return MSG_M_ERROR_FAILED_CREATE_SOCKET;
    }
    //printf("socket created, fd %d\n", socket_fd);

    // Specify server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");    // Connect to localhost

    // Connect to server
    if(connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        //printf("connect failed\n");
        state = MSG_M_ST_ERR;
        return MSG_M_ERROR_FAILED_CONN;
    }

    // Send alive message
    Message_t alive_message(id, ROUTER, ALIVE);

    // We need to serialize the message into a buffer before sending it
    uint8_t alive_message_buffer[Message_t::max_message_size];    // NOTE: watch stack memory usage
    uint16_t alive_message_size;

    alive_message.to_bytes(alive_message_buffer, alive_message_size);

    if(fcntl(socket_fd, F_GETFD) == -1 && errno == EBADF)
    {
        //printf("Invalid file descriptor 1\n");
        return MSG_M_ERROR_FAILED_CONN;
    }

    ssize_t res = send(socket_fd, alive_message_buffer, alive_message_size, 0);

    if(fcntl(socket_fd, F_GETFD) == -1 && errno == EBADF)
    {
        //printf("Invalid file descriptor 2\n");
        return MSG_M_ERROR_FAILED_CONN;
    }

    if(res == -1)
    {
        //printf("alive send failed\n");
        return MSG_M_ERROR_SEND_FAILED;
    }

    // res should be equal to the number of bytes sent
    if((size_t)res != alive_message_size)
    {
        //printf("alive sent failed\n");
        return MSG_M_ERROR_SEND_FAILED;
    }

    //cout << "connected" << endl;

    state = MSG_M_ST_CONN;

    // starting receiving_thread
    // pthread_create(&receiving_thread, NULL, receive_message, this);

    return MSG_M_ERROR_NONE;
}

/* Sends the packet in the argument to established communication */
Message_manager_error_t Message_manager_t::send_message(Message_t &message)
{
    // Check if connected
    if(!compare_state(MSG_M_ST_CONN))
        return MSG_M_ERROR_NOT_CONN;

    // TODO: check if packet source id is the same as the id of the manager

    // Serialize the message into a buffer before sending it
    uint8_t packet_buffer[Message_t::max_message_size];    // TODO: this buffer is too big to just declare like this
    uint16_t packet_size;

    message.to_bytes(packet_buffer, packet_size, false);

    // Send the packet header
    ssize_t sent_bytes = send(socket_fd, packet_buffer, message.get_header_size(), 0);

    printf("Sending %d header bytes\n", (int)sent_bytes);
    if(sent_bytes < 0)
    {
        printf("send failed %d\n", errno);
        return MSG_M_ERROR_SEND_FAILED;
    }

    // If data is not empty, send data
    if(message.get_data_size() != 0)
    {
        message.to_bytes(packet_buffer, packet_size, true);
        sent_bytes += send(socket_fd, packet_buffer + message.get_header_size(), message.get_data_size(), 0);
    }

    // Check if sent bytes equals the amount that should have been sent (header
    // + data)
    if((size_t)sent_bytes != message.get_size())
    {
        return MSG_M_ERROR_INCOMP_PACK;
    }

    return MSG_M_ERROR_NONE;
}

/* Waits (blocking) until a message is sent, then loads the received data into
 * the packet */
Message_manager_error_t Message_manager_t::wait_and_receive_message(Message_t &message)
{
    // Wait (blocking) until a message is sent
    await_new_messsage();

    // Receive the header into a buffer
    uint8_t buffer[Message_t::max_message_size];
    ssize_t len = recv(socket_fd, buffer, message.get_header_size(), 0);

    // Deserialize the header from the buffer, we don't copy the data, just the header
    message.from_bytes(buffer, false);

    // TODO: Is this expected behaviour?
    if(len == 0 || len == -1)    // NOTE: added check for read errors, is this correct?
    {
        if(len == -1)
            printf("error receiving with errno: %d\n", errno);
        state = MSG_M_ST_NOT_CONN;
        return MSG_M_ERROR_NOT_CONN;
    }

    // If the packet has data, receive it
    if(message.get_data_size() != 0)
    {
        // receive the packet's data in the buffer
        len = recv(socket_fd, buffer, message.get_data_size(), MSG_WAITALL);
        if((size_t)len != message.get_data_size())
        {
            return MSG_M_ERROR_INCOMP_PACK;
        }

        // Deserialize the data from the buffer
        message.from_bytes(buffer, true);
    }

    return MSG_M_ERROR_NONE;
}

/* Closes the socket connection and changes the state accordingly */
Message_manager_error_t Message_manager_t::disconnect()
{
    if(!compare_state(MSG_M_ST_CONN))
        return MSG_M_ERROR_NOT_CONN;

    close(socket_fd);
    state = MSG_M_ST_NOT_CONN;

    // pthread_join(receiving_thread, NULL);
    //cout << "disconnected" << endl;

    return MSG_M_ERROR_NONE;
}

/* Returns the current state */
Message_manager_state_t Message_manager_t::get_state()
{
    return state;
}

/* Constructor that is called when object is created */
Message_manager_t::Message_manager_t()
{
    state = MSG_M_ST_NOT_CONN;
    socket_fd = -1;
    // isr_func = NULL;
    id = UNDF;
}

/* Destructor that is called when object is out of scope or deleted */
Message_manager_t::~Message_manager_t()
{
    disconnect();
}

/* Sets the receive_handler */
void Message_manager_t::set_receive_handler(receive_handler handler)
{
    this->handler = handler;
}

/* Returns the receive_handler */
receive_handler Message_manager_t::get_receive_handler()
{
    return this->handler;
}

/* Set id of this manager instance */
void Message_manager_t::set_id(App_id_t id)
{
    this->id = id;
}
