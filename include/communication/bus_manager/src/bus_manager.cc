#include <cstdio>
#include <cstring>
#ifndef __MSP430FR5969__
#include <unistd.h>
#endif

#include "bus_manager.h"

/* Set BUS_MANAGER_TESTING variable to 1 to activate testing code. */
// TODO: This is bad practice, refactor this
#if BUS_MANAGER_TESTING == 1
#define BUS0 "/dev/ttyS0"
#endif

Bus_manager_t::Bus_manager_t(const Bus_t bus, const subsystems::Subsystem_t source)
    : initialised(0u), bus(bus), bus_tty((bus == BUS_MANAGER_BUS0)   ? BUS0
                                         : (bus == BUS_MANAGER_BUS1) ? BUS1
                                         : (bus == BUS_MANAGER_BUS2) ? BUS2
                                         : (bus == BUS_MANAGER_BUS3) ? BUS3
                                                                     : BUS4),
      stored_packet(subsystems::NONE, 0, Packet_t::RETRANSMISSION), sn_start(0u), nesn_start(0u), source(source), is_subsystem(source > subsystems::Subsystem_t::PPU) /* Decide if it is a subsystem or not (only OBC and PPU are not subsystems). */
{
}

Bus_manager_t::~Bus_manager_t()
{
    this->close();
}

int32_t Bus_manager_t::configure_serial(const Baudrate_t &baudrate, const Serial_mode_t &mode)
{
    return serial_configure(this->bus_tty, baudrate, mode, &(this->bus_fd));
}

uint32_t Bus_manager_t::write_to_serial(uint8_t packet_raw[Packet_t::PACKET_MAX_LENGTH], Packet_t &packet)
{
    return serial_write(this->bus_fd, (char *)packet_raw, Packet_t::PACKET_HEAD_LENGTH + packet.pdu.header.msb.length + Packet_t::PACKET_CRC_LENGTH);
}

uint32_t Bus_manager_t::read_from_serial(uint8_t packet_raw[Packet_t::PACKET_MAX_LENGTH], size_t *length, const uint32_t timeout_milliseconds)
{
    return serial_read(this->bus_fd, reinterpret_cast<char *>(packet_raw), length, timeout_milliseconds);
}

void Bus_manager_t::close(void)
{
    /* Close serial bus. */
    serial_close(this->bus_fd);
}

Bus_manager_t::Result_t Bus_manager_t::init(const Baudrate_t &baudrate, const Serial_mode_t &mode)
{
    Result_t status = construct_result(Error_code_t::NO_ERROR);
    uint32_t configure_serial_result;

    /* Only initialise bus when it hasn't been done. */
    if(!this->initialised)
    {
        /* Configure the serial bus. */
        configure_serial_result = configure_serial(baudrate, mode);
        if(configure_serial_result != 0u)
        {
            status = construct_result(Error_code_t::SERIAL_CONFIGURE_ERROR, configure_serial_result);
        }
        else
        {
            /* Set bus to initialised while semaphore is still claimed. */
            this->initialised = 1u;
        }
    }
    else
    {
        status = construct_result(Error_code_t::ALREADY_INITIALISED);
    }

    return status;
}

Bus_manager_t::Result_t Bus_manager_t::read_from_bus(Packet_t &message)
{
    return this->read_from_bus(&message, 1u);
}

Bus_manager_t::Result_t Bus_manager_t::read_from_bus(Packet_t message[], const uint32_t max_expected_packets)
{
    /* This packet is a placeholder for the interface_read function. */
    Packet_t request_packet(subsystems::Subsystem_t::NONE, 0u, Packet_t::RETRANSMISSION);
    Result_t status = construct_result(Error_code_t::NO_ERROR);

    /* Error if the bus manager is not initialised. */
    if(this->initialised == 0u)
    {
        status = construct_result(Error_code_t::NOT_YET_INITIALISED);
    }
    else
    {
        /* Set SN and NESN values. */
        this->sn = 0u;
        this->previous_sn = !this->sn;
        this->nesn = 0u;
        this->previous_nesn = this->nesn;

        this->max_packets_to_read = max_expected_packets;

        /* Read from the bus. */
        status = this->interface_read(request_packet, message, max_expected_packets);

        if(status.error_code == Error_code_t::NO_ERROR)
        {
            /* Set next NESN and SN for write interfacing. */
            this->sn_start = this->sn;
            this->nesn_start = this->nesn;
        }
    }

    /* Return status result. */
    return status;
}

Bus_manager_t::Result_t Bus_manager_t::interface(Packet_t &request_packet, const uint32_t timeout_milliseconds)
{
    /* Dummy packet. */
    Packet_t response_packet;

    return interface(&request_packet, 1u, &response_packet, 1u, timeout_milliseconds, 0u);
}

Bus_manager_t::Result_t Bus_manager_t::interface(Packet_t request_packets[], uint32_t num_request_packets, const uint32_t timeout_milliseconds)
{
    /* Dummy packet. */
    Packet_t response_packet;

    return interface(request_packets, num_request_packets, &response_packet, 1u, timeout_milliseconds, 0u);
}

Bus_manager_t::Result_t Bus_manager_t::interface(Packet_t &request_packet, Packet_t &response_packet, const uint32_t timeout_milliseconds)
{
    return interface(&request_packet, 1u, &response_packet, 1u, timeout_milliseconds, 1u);
}

Bus_manager_t::Result_t Bus_manager_t::interface(Packet_t &request_packet, Packet_t response_packets[], const uint32_t max_num_response_packets, const uint32_t timeout_milliseconds)
{
    return interface(&request_packet, 1u, response_packets, max_num_response_packets, timeout_milliseconds, 1u);
}

Bus_manager_t::Result_t Bus_manager_t::interface(Packet_t request_packets[], const uint32_t num_request_packets, Packet_t &response_packet, const uint32_t timeout_milliseconds)
{
    return interface(request_packets, num_request_packets, &response_packet, 1u, timeout_milliseconds, 1u);
}

Bus_manager_t::Result_t Bus_manager_t::interface(Packet_t request_packets[], const uint32_t num_request_packets, Packet_t response_packets[], const uint32_t max_num_response_packets, const uint32_t timeout_milliseconds, const uint8_t expect_reply)
{
    Result_t status = construct_result(Error_code_t::NO_ERROR);

    /* Set retries to 0. */
    this->retransmission_retries = 0u;
    this->retransmission_request_retries = 0u;

    this->max_packets_to_read = max_num_response_packets;
    this->timeout_milliseconds = timeout_milliseconds;

    /* Set NESN to 1 if slave, 0 if master. */
    this->nesn = this->nesn_start;
    this->previous_nesn = this->nesn;
    this->sn = this->sn_start;
    this->previous_sn = !this->sn;

    /**
     * Return immediately if the bus is not yet initialised.
     */
    if(this->initialised == 0u)
    {
        status = construct_result(Error_code_t::NOT_YET_INITIALISED);
    }
    else
    {
        if(status.error_code == Error_code_t::NO_ERROR)
        {
            /**
             * Write all messages (with multiple data if required).
             * Each request message expects an ACK.
             * The last request message expects an ACK or REPLY depending on the
             * value of expect_reply.
             */
            for(uint32_t request_packets_index = 0; request_packets_index < num_request_packets; request_packets_index++)
            {
                Packet_t &request_packet = request_packets[request_packets_index];

                /* Set preamble of request packet. */
                request_packet.preamble = Packet_t::PACKET_PREAMBLE;

                /* Set NESN and SN to 0. */
                request_packet.pdu.header.lsb.nesn = this->nesn;
                request_packet.pdu.header.lsb.sn = this->sn;

                /* Set the request packet multiple data to 1 if it is not the
                 * last packet. */
                if(request_packets_index < (num_request_packets - 1u))
                {
                    request_packet.pdu.header.lsb.md = 1u;
                }

                /* Do read/write operations for the final write. After this, we
                 * expect a reply from the destination. */
                if((request_packets_index == (num_request_packets - 1u)) && expect_reply)
                {
                    status = this->interface_operations(request_packet, response_packets, max_num_response_packets);
                }
                /* We only expect an ACK. */
                else
                {
                    /* Do read/write operations. */
                    status = this->interface_operations_ack(request_packet);
                }

                /* If, at any point, we encounter an error, we stop the
                 * interfacing.
                 */
                if(status.error_code != Error_code_t::NO_ERROR)
                {
                    break;
                }
            }
        }
    }

    return status;
}

Bus_manager_t::Result_t Bus_manager_t::interface_operations_ack(Packet_t &request_packet)
{
    /* Dummy packet. */
    Packet_t response_packet;

    return interface_operations(request_packet, &response_packet, 1u);
}

Bus_manager_t::Result_t Bus_manager_t::interface_operations(Packet_t &request_packet, Packet_t response_packets[], const uint32_t remaining_packets)
{
    Result_t status;

    /* Write request packet onto the bus. */
    status = this->write(request_packet);
    if(status.error_code == Error_code_t::NO_ERROR)
    {
        status = interface_read(request_packet, response_packets, remaining_packets);
    }

    return status;
}

Bus_manager_t::Result_t Bus_manager_t::interface_read(Packet_t &request_packet, Packet_t response_packets[], const uint32_t remaining_packets)
{
    Result_t status;
    uint8_t check_response_value;
    uint32_t remaining_packets_local = remaining_packets;
    uint32_t packet_index = 0;

    /* Used in retransmission request. */
    Packet_t *last_written_packet = &request_packet;

    /* Used to signal that there was a recusrive call and we should return. */
    uint8_t recursed = 0u;

    /* Set the destination for the packets. For a subsystem, this will be NONE.
     */
    this->destination = static_cast<subsystems::Subsystem_t>(request_packet.destination);

    /* Keep reading while there is more data. */
    do
    {
        /* Set packet index. */
        packet_index = this->max_packets_to_read - remaining_packets_local;

        /* Set response packet to all zeroes. */
        response_packets[packet_index].clear_all_fields();

        /* Read response packet. */
        status = this->read(response_packets[packet_index]);

        /* Send retransmission request if the CRC is invalid or the read timed
         * out.
         */
        if((status.error_code == Error_code_t::INVALID_CRC) || (status.error_code == Error_code_t::SERIAL_TIMEOUT_ERROR))
        {
            /* If we don't know the destination yet because it is the first
             * message, give an error because we cannot send a targeted message.
             * This only occurs on a subsystem, since we do not know if the message
             * comes from the PPU or the OBC before we decode it.
             */
            if(this->destination == subsystems::Subsystem_t::NONE)
            {
                status = construct_result(Error_code_t::UNKNOWN_PACKET_DESTINATION, (int32_t)status.error_code);
            }
            else
            {
                /* Check if we are still under the retry limit. */
                if(++this->retransmission_request_retries < BUS_RETRIES)
                {
                    /* Prepare retransmission packet. */
                    construct_retransmission_packet();

                    /* Send a retransmission request. */
                    status = this->interface_operations(this->stored_packet, response_packets, remaining_packets_local);
                }
                else
                {
                    /* Retry limit exceeded. */
                    status = construct_result(Error_code_t::RETRY_LIMIT_REACHED, (int32_t)status.error_code);
                }

                recursed = 1u;
            }
        }
        else if(status.error_code == Error_code_t::NO_ERROR)
        {
            /* Check the response. */
            check_response_value = check_response(response_packets[packet_index]);

            /* Request not ACK'd, try retransmission. */
            if(check_response_value != 0u)
            {
                /* If we don't know the destination yet because it is the first
                 * message, give an error because we cannot send a targeted
                 * message. */
                if(this->destination == subsystems::Subsystem_t::NONE)
                {
                    status = construct_result(Error_code_t::UNKNOWN_PACKET_DESTINATION, (int32_t)status.error_code);
                }
                else
                {
                    /* Check if we are still under the retry limit. */
                    if(++this->retransmission_retries < BUS_RETRIES)
                    {
                        /* Try sending the packet again. */
                        status = this->interface_operations(*last_written_packet, response_packets, remaining_packets_local);
                    }
                    else
                    {
                        /* Retry limit exceeded. */
                        status = construct_result(Error_code_t::RETRY_LIMIT_REACHED, (int32_t)Error_code_t::NACK);
                    }

                    recursed = 1u;
                }
            }
            else
            {
                /* Set the destination according to the source of the response.
                 */
                this->destination = static_cast<subsystems::Subsystem_t>(response_packets[packet_index].source);

                /* Only acknowledge packets that:
                 *   - are not ACKs
                 *   - are not the last packet sent when is_subsystem is true
                 * (we want to send a reply later instead).
                 */
                if((response_packets[packet_index].pdu.header.lsb.type != Packet_t::ACK_NACK) && !((response_packets[packet_index].pdu.header.lsb.md == 0u) && this->is_subsystem))
                {
#ifndef __MSP430FR5969__
                    /* Necessary otherwise the MSP will time out.
                     * This is the minimum possible delay determined from testing.
                     */
                    usleep(1000);
#endif

                    /* Construct ACK packet. */
                    construct_ack_packet();

                    /* Send ACK packet to the destination. */
                    status = this->write(this->stored_packet);
                    if(status.error_code == Error_code_t::NO_ERROR)
                    {
                        /* Set last written packet to ack_nack: used for
                         * retransmission. */
                        last_written_packet = &this->stored_packet;

                        /* Decrease remaining packets. */
                        remaining_packets_local--;
                    }
                }
            }
        }
        /* Empty else statement required by MISRA. */
        else
        {
        }

        /* Break out of the loop if we have an error or made a recursive call.
         */
        if((status.error_code != Error_code_t::NO_ERROR) || (recursed == 1u))
        {
            break;
        }
    } while((remaining_packets_local > 0u) && (response_packets[packet_index].pdu.header.lsb.md == 1u));

    /* If MD is still set and there is no more data, signal an error. */
    if(!((status.error_code != Error_code_t::NO_ERROR) || (recursed == 1u)) && (response_packets[packet_index].pdu.header.lsb.md == 1u))
    {
        status = construct_result(Error_code_t::UNEXPECTED_MULTIPLE_DATA);
    }

    return status;
}

Bus_manager_t::Result_t Bus_manager_t::write(Packet_t &packet)
{
    uint32_t write_status;
    uint8_t packet_raw[Packet_t::PACKET_MAX_LENGTH] = {0};
    Result_t status = Result_t{Error_code_t::NO_ERROR, 0};

    /* Set packet source. */
    packet.source = this->source;

    /* Calculate packet CRC. */
    packet.calculate_crc(packet_raw);

    /* Convert packet to raw bytes. */
    packet.to_bytes(packet_raw);

    /* Write packet to bus. */
    write_status = write_to_serial(packet_raw, packet);
    if(write_status != 0u)
    {
        status = construct_result(Error_code_t::SERIAL_WRITE_ERROR, write_status);
    }

    /* Flip sequence number. */
    this->previous_sn = packet.pdu.header.lsb.sn;
    this->previous_nesn = packet.pdu.header.lsb.nesn;
    this->sn = !this->previous_sn;

    return status;
}

Bus_manager_t::Result_t Bus_manager_t::read(Packet_t &packet)
{
    Result_t status = construct_result(Error_code_t::NO_ERROR);
    uint32_t read_status;
    uint8_t packet_raw[Packet_t::PACKET_MAX_LENGTH] = {0};

    /* Received length according to Serial library */
    size_t length;

    /* Calculated CRC over the received data. */
    uint32_t raw_crc;

    /* Read packet response */
    read_status = read_from_serial(packet_raw, &length, this->timeout_milliseconds);
    if(read_status == 2u)
    {
        status = construct_result(Error_code_t::SERIAL_TIMEOUT_ERROR);
    }
    else if(read_status == 6u)
    {
        status = construct_result(Error_code_t::INVALID_PACKET_DESTINATION, packet.destination);
    }
    else if(read_status != 0u)
    {
        status = construct_result(Error_code_t::SERIAL_READ_ERROR, read_status);
    }
    else
    {
        /* Convert to packet. */
        packet.from_bytes(packet_raw);

        /* Check if the destination on the packet is equal to the subsystem of
         * the bus manager. */
        if((packet.destination) != (this->source))
        {
            status = construct_result(Error_code_t::INVALID_PACKET_DESTINATION, packet.destination);
        }
        else
        {
            /* Store the raw byte crc to compare later. */
            raw_crc = packet.crc;

            /* Calculate CRC from read message. Also sets the CRC which is why
             * the previous line is necessary. */
            packet.calculate_crc(packet_raw, length);

            /* Compare calculated CRC to packet CRC. */
            if(raw_crc != packet.crc)
            {
                status = construct_result(Error_code_t::INVALID_CRC, raw_crc);
            }

            /* Set NESN to inverse of received packet SN. */
            this->nesn = !packet.pdu.header.lsb.sn;
        }
    }

    return status;
}

void Bus_manager_t::construct_retransmission_packet(void)
{
    this->stored_packet.pdu.header.lsb.type = Packet_t::RETRANSMISSION;
    this->stored_packet.source = this->source;
    this->stored_packet.destination = this->destination;
    this->stored_packet.pdu.header.lsb.sn = this->previous_sn;
    this->stored_packet.pdu.header.lsb.nesn = this->previous_nesn;
}

void Bus_manager_t::construct_ack_packet(void)
{
    this->stored_packet.pdu.header.lsb.type = Packet_t::ACK_NACK;
    this->stored_packet.source = this->source;
    this->stored_packet.destination = this->destination;
    this->stored_packet.pdu.header.lsb.nesn = this->nesn;
    this->stored_packet.pdu.header.lsb.sn = this->sn;
}

Bus_manager_t::Result_t Bus_manager_t::construct_result(const Error_code_t error_code, const int32_t error_details)
{
    return Result_t{error_code, error_details};
}

uint8_t Bus_manager_t::check_response(const Packet_t &response_packet) const
{
    uint8_t result = 0u;

    /* If the previous SN is the same as the response NESN, then the request was
     * not ACK'd and needs to be retransmitted.
     */
    if((response_packet.pdu.header.lsb.type == Packet_t::RETRANSMISSION) || (this->previous_sn == response_packet.pdu.header.lsb.nesn))
    {
        result = 1u;
    }

    /* If the previous NESN is not the same as the response SN, the data is
     * invalid.
     */
    if(this->previous_nesn != response_packet.pdu.header.lsb.sn)
    {
        result = 2u;
    }

    return result;
}
