#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include <iostream>
#include <communication/message_manager/src/message.h>
#include <time.h>

// Note to future readers, this class is in severe need of a rewrite, it was found undocumented and untested.
// The message class was built after this one, and this one was bodged to work with it.
// I believe the router_packet class was trying to implement something similar, but due to lack of documentation i am unsure
// Some comments were left by me, others were left by Davis after he tried to figure out what's going on here.
//      -Good luck, Ionut

/* Function signature for the receive handler function, that is responsible
 * for handling received packets */
typedef void (*receive_handler)(Message_t *received, Message_t *sent);

/* Enum for the states of the message manager */
typedef enum
{
    MSG_M_ST_CONN = 0,
    MSG_M_ST_NOT_CONN,
    MSG_M_ST_INIT,
    MSG_M_ST_ERR
} Message_manager_state_t;

/* Enum for the errors of the message manager */
typedef enum
{
    MSG_M_ERROR_NONE = 0,
    MSG_M_ERROR_TIMEOUT = 1,
    MSG_M_ERROR_FAILED_CONN = 2,
    MSG_M_ERROR_FAILED_CREATE_SOCKET = 3,
    MSG_M_ERROR_SEND_FAILED = 4,
    MSG_M_ERROR_NOT_CONN = 5,
    MSG_M_ERROR_NULL_DATA_B = 6,
    MSG_M_ERROR_INCOMP_PACK = 7,
    MSG_M_ERROR_NULL_DATA = 8,
} Message_manager_error_t;

class Message_manager_t
{
  private:
    // Current state of the message manager
    Message_manager_state_t state;
    // Socket's file descriptor
    int socket_fd;
    // Handler that is used to handle received packets
    receive_handler handler;
    // Id of the application using the message manager
    App_id_t id;

    /* get next buffer addr */
    uint8_t *next_buffer();

    /* Sets up the file descriptor set and waits (blocking) until a
     * message is sent */
    void await_new_messsage();

  public:
    /*** Constructor and deconstructor ***/
    Message_manager_t();
    ~Message_manager_t();

    /* interrupt callback function */
    // void (*isr_func)(uint8_t, uint8_t, void *);

    /*** Callable functions  ***/
    Message_manager_error_t connect_to_port(uint16_t);
    Message_manager_error_t disconnect();

    Message_manager_error_t send_message(Message_t &message);

    Message_manager_error_t send_and_await_response(Message_t &message, Message_t &response, uint16_t timeout);
    /*** Async functions used by the receiving_thread that captures and
     * handles incoming packets  ***/
    Message_manager_error_t wait_and_receive_message(Message_t &message);

    bool compare_state(Message_manager_state_t);

    /*** Getters and setters ***/
    Message_manager_state_t get_state();
    receive_handler get_receive_handler();
    // void subsr_isr(void(*func)(uint8_t,uint8_t,void*));
    void set_receive_handler(receive_handler handler);
    void set_id(App_id_t id);
};
#endif
