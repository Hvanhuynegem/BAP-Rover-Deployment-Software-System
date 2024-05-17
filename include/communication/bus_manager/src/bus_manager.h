/**
 * Author(s):
 *    Marco van Eerden
 *
 * The Bus_manager_t class implements the BLE-derived messaging protocol that
 * the OBC uses to communicate with the subsystems and vice versa. Each
 * Bus_manager_t instance is linked to a single bus.
 *
 * There are three public methods that can be used externally:
 *    1. init --> used to initialise the bus manager and serial bus.
 *    2. read_from_bus --> used in subsystems to listen to messages from the OBC
 * or PPU.
 *    3. interface --> used to write requests or replies to the bus, and receive
 * ACKs.
 *
 * For any packet you want to send, you only need to specify the destination,
 * payload length and type. Everything else is handled by the bus manager.
 */
#pragma once

#include "packet.h"
#include "subsystems.h"

#ifdef __MSP430FR5969__
#include <communication/serial/src/msp_serial.h>
#else
#include "serial.h"
#endif

/* Set BUS_MANAGER_TESTING to 1 to activate testing code. */
#define BUS_MANAGER_TESTING 1

/* Number of bus retries. */
#define BUS_RETRIES 5u    // TODO: subject to change

/* Number of buses on the rover. */
#define NUM_BUSES 5u

class Bus_manager_t
{
  public:
    /* Errors that can be 'thrown' by the bus manager. */
    enum Error_code_t : uint8_t
    {
        NO_ERROR = 0u,
        ALREADY_INITIALISED = 1u,
        SERIAL_CONFIGURE_ERROR = 2u,
        SERIAL_WRITE_ERROR = 3u,
        SERIAL_READ_ERROR = 4u,
        SERIAL_TIMEOUT_ERROR = 5u,
        INVALID_CRC = 6u,
        RETRY_LIMIT_REACHED = 7u,
        UNEXPECTED_MULTIPLE_DATA = 8u,
        NACK = 9u,
        NOT_YET_INITIALISED = 10u,
        INVALID_PACKET_DESTINATION = 11u,
        UNKNOWN_PACKET_DESTINATION = 12u,
        BUS_OCCUPIED = 13
    };

    /* Structure used for returning errors. */
    struct Result_t
    {
        Error_code_t error_code;
        int32_t error_details;
    };

    /* Bus numbers. */
    enum Bus_t
    {
        BUS_MANAGER_BUS0 = 0u,
        BUS_MANAGER_BUS1 = 1u,
        BUS_MANAGER_BUS2 = 2u,
        BUS_MANAGER_BUS3 = 3u,
        BUS_MANAGER_BUS4 = 4u
    };

    /**
     * Constructor.
     *
     * @param bus    Bus number.
     * @param source Source subsystem.
     */
    Bus_manager_t(const Bus_t bus, const subsystems::Subsystem_t source);

    /**
     * Destructor.
     * Closes the serial bus.
     */
    ~Bus_manager_t(void);

    /**
     * Initialises the bus manager by configuring the serial bus.
     * Can only be initialised once and needs to be initialised to do an
     * operation.
     *
     * @param baudrate Baud rate for the bus.
     * @param mode     Serial mode for the bus.
     *
     * @return Error code when an error occurred, or NO_ERROR otherwise.
     */
    Result_t init(const Baudrate_t &baudrate, const Serial_mode_t &mode);

    /**
     * Read a single-packet message from the bus.
     * Bus manager must be initialised for operations to occur.
     *
     * @param message Received bytes are stored in here.
     *
     * @return Status result.
     */
    Result_t read_from_bus(Packet_t &message);

    /**
     * Read a message from the bus that consists of multiple packets.
     * Bus manager must be initialised for operations to occur.
     *
     * @param message              Array of message packets.
     * @param max_expected_packets Maximum expected packets. MUST be smaller
     * than or equal to array length!
     *
     * @return Status result.
     */
    Result_t read_from_bus(Packet_t message[], const uint32_t max_expected_packets);

    /**
     * Interface function for a single request packet without expecting a
     * reply.
     *
     * @param request_packet       Packet to write to the bus.
     * @param timeout_milliseconds Read timeout in milliseconds.
     *
     * @return Status result.
     */
    Result_t interface(Packet_t &request_packet, const uint32_t timeout_milliseconds);

    /**
     * Interface function for multiple request packets without expecting a
     * reply.
     *
     * @param request_packets      Array of packets to write to the bus.
     * @param num_request_packets  Length of request packets arrray.
     * @param timeout_milliseconds Read timeout in milliseconds.
     *
     * @return Status result.
     */
    Result_t interface(Packet_t request_packets[], const uint32_t num_request_packets, const uint32_t timeout_milliseconds);

    /**
     * Interfacing function for a single request and single response packet.
     *
     * @param request_packet       Packet to write to the bus.
     * @param response_packet      Response is stored in here.
     * @param timeout_milliseconds Read timeout in milliseconds.
     *
     * @return Status result.
     */
    Result_t interface(Packet_t &request_packet, Packet_t &response_packet, const uint32_t timeout_milliseconds);

    /**
     * Interfacing function for a single request packet and multiple
     * response packets.
     *
     * @param request_packet           Packet to write to the bus.
     * @param response_packets         Response is stored in this array.
     * @param max_num_response_packets Maximum expected number of response
     * packets.
     * @param timeout_milliseconds     Read timeout in milliseconds.
     *
     * @return Status result.
     */
    Result_t interface(Packet_t &request_packet, Packet_t response_packets[], const uint32_t max_num_response_packets, const uint32_t timeout_milliseconds);

    /**
     * Interfacing function for multiple request packets and a single
     * response packet.
     *
     * @param request_packets      Array of packets to write to the bus.
     * @param num_request_packets  Number of request packets.
     * @param response_packet      Response is stored in here.
     * @param timeout_milliseconds Read timeout in milliseconds.
     *
     * @return Status result.
     */
    Result_t interface(Packet_t request_packets[], const uint32_t num_request_packets, Packet_t &response_packet, const uint32_t timeout_milliseconds);

    /**
     * Interfaces between the source and destination by writing requests and
     * reading the response to each request (either ACK or REPLY).
     *
     * While this operation is ongoing, no other device should get access to
     * the bus.
     *
     * @param request_packets          Array of packets to write to the bus.
     * @param num_request_packets      Number of request packets.
     * @param response_packets         Response is stored in this array.
     * @param max_num_response_packets Maximum expected number of response
     * packets.
     * @param timeout_milliseconds     Read timeout in milliseconds.
     * @param expect_reply             If 1, expect a reply after the last
     * request message is sent. Else, only expect ACKs.
     *
     * @return Status result.
     */
    Result_t interface(Packet_t request_packets[], const uint32_t num_request_packets, Packet_t response_packets[], const uint32_t max_num_response_packets, const uint32_t timeout_milliseconds, const uint8_t expect_reply = 1u);

  protected:
    /* Used to facilitate exclusive bus access. */
    enum Bus_state_t
    {
        IDLE = 0u,
        BUSY = 1u
    } state;

    /* If the bus is initialised or not. */
    uint8_t initialised;

    /* Serial file descriptor. */
    int bus_fd;

    /* Number of retransmission retries. */
    uint8_t retransmission_retries;
    /* Number of retransmission requests before giving up. */
    uint8_t retransmission_request_retries;

    /* Bus number. */
    Bus_t bus;
    /* Bus TTY string. */
    const char *bus_tty;

    /* Packet used for retransmission requests and ACK/NACK. */
    Packet_t stored_packet;

    /* Maximum packets to read. */
    uint32_t max_packets_to_read;

    /* Sequence number (SN). */
    uint8_t sn;
    /* Previously sent SN. */
    uint8_t previous_sn;
    /* SN to use at start of interfacing. Used to get the correct SN when
     * reading before interfacing. */
    uint8_t sn_start;

    /* Next expected sequence number (NESN). */
    uint8_t nesn;
    /* Previously sent NESN. */
    uint8_t previous_nesn;
    /* NESN to use at start of interfacing. Used to get the correct NESN
     * when reading before interfacing. */
    uint8_t nesn_start;

    /* Read timeout in milliseconds. */
    uint32_t timeout_milliseconds;

    /* Subsystem that the bus manager is part of. */
    const subsystems::Subsystem_t source;
    /* Destination of the messages. */
    subsystems::Subsystem_t destination;

    /* If the bus manager is part of a subsystem or not. */
    uint8_t is_subsystem;

    /**
     * Interfacing function used for writing a packet with an ACK as
     * expected reply. Here, the response packet is only used to store the
     * ACK from the destination so it can be discarded afterwards.
     *
     * @param request_packet Packet to write to the bus.
     *
     * @return Status result.
     */
    Result_t interface_operations_ack(Packet_t &request_packet);

    /**
     * Entry point for the interfacing.
     * The request packet is written to the bus. If everything is OK, we
     * read the response.
     *
     * @param request_packet        Request packet to send.
     * @param response_packet       Packet to store response into.
     * @param timeout_milliseconds  Read timeout in milliseconds.
     *
     * @return Error code if error occurred or NO_ERROR otherwise.
     */
    Result_t interface_operations(Packet_t &request_packet, Packet_t response_packet[], const uint32_t remaining_packets);

    /**
     * Interfaces source and destination in the following steps:
     *  1. Read data from the bus. If CRC is invalid or read timed out, send
     * retransmission request.
     *  2. Check the response: if it is a retransmission request, retransmit
     * the frame.
     *  3. Transmit an ACK message to signal that the response was received
     * correctly.
     *  4. Keep reading while there is more data.
     */
    Result_t interface_read(Packet_t &request_packet, Packet_t response_packets[], const uint32_t remaining_packets);

    /**
     * Convert a packet to bytes and write it to the serial bus.
     * Sets the packet source and calculates the CRC.
     * Flips the SN bit after a write.
     *
     * @param packet Packet to send.
     *
     * @return SERIAL_WRITE_ERROR if an error occurred, NO_ERROR otherwise.
     */
    Result_t write(Packet_t &bus_packet);

    /**
     * Read a packet from the serial bus.
     *
     * @param packet Packet to store the response into.
     *
     * @return Error code if an error occurred, NO_ERROR otherwise.
     */
    Result_t read(Packet_t &bus_packet);

    /**
     * Configure the serial bus.
     *
     * @param baudrate Baudrate to be used on the bus.
     * @param mode     Bus mode (blocking or non-blocking).
     *
     * @return 0 on success, not 0 otherwise.
     */
    virtual int32_t configure_serial(const Baudrate_t &baudrate, const Serial_mode_t &mode);

    /**
     * Write to the serial bus.
     *
     * @param packet_raw Packet in raw bytes.
     * @param packet     Packet in struct form.
     *
     * @return 0 on success, not 0 otherwise.
     */
    virtual uint32_t write_to_serial(uint8_t packet_raw[Packet_t::PACKET_MAX_LENGTH], Packet_t &packet);

    /**
     * Read from the serial bus.
     *
     * @param packet_raw           Packet in raw bytes.
     * @param length               Serial_read writes the length of the
     * received packet here.
     * @param timeout_milliseconds Read timeout in milliseconds.
     *
     * @return 2u on timeout, 0u on success, other value otherwise.
     */
    virtual uint32_t read_from_serial(uint8_t packet_raw[Packet_t::PACKET_MAX_LENGTH], size_t *length, const uint32_t timeout_milliseconds);

    /**
     * Closes the serial bus.
     */
    virtual void close(void);

    /**
     * Construct the retransmission request packet.
     */
    void construct_retransmission_packet(void);

    /**
     * Construct an ACK packet.
     */
    void construct_ack_packet(void);

    /**
     * Construct a details struct with the required error code and details.
     *
     * @param error_code    Previously sent packet.
     * @param error_details Received packet.
     *
     * @return Status result.
     */
    static Result_t construct_result(const Error_code_t error_code, const int32_t error_details = 0);

    /**
     * Return 1 if the request was not acknowledged, 2 if the data is
     * invalid and 0 otherwise.
     *
     * For more info about SN and NESN, see:
     *
     * https://novelbits.io/understanding-sn-nesn-ble-link-layer-packet/
     *
     * @param response_packet Packet that was received.
     *
     * @return 1 if request not acknowledged, 2 if data invalid, 0
     * otherwise.
     */
    uint8_t check_response(const Packet_t &response_packet) const;
};
