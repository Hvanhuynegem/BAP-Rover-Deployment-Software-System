/**
 * Author(s):
 *    Marco van Eerden
 *
 * Contains the Packet_t class, which represents a message packet.
 */

#pragma once

#include "subsystems.h"

#include <stdint.h>
#include <stdlib.h>

/**
 * Packet_t represents a message packet with preamble, source, destination, PDU
 * and CRC. Packets can be converted to and from raw bytes conveniently using
 * this class.
 */
class Packet_t
{
  public:
    static const uint16_t PACKET_CRC_LENGTH = 3u;
    static const uint8_t PACKET_PREAMBLE = 0x3Au;
    static const uint16_t PACKET_HEAD_LENGTH = 5u;
    static const uint16_t PACKET_MAX_PAYLOAD_LENGTH = 256u;
    static const uint16_t PACKET_MAX_LENGTH = PACKET_HEAD_LENGTH + PACKET_MAX_PAYLOAD_LENGTH + PACKET_CRC_LENGTH;

    /* Packet types. */
    enum Packet_type_t : uint8_t
    {
        COMMAND = 0b00u,
        RETRANSMISSION = 0b01u,
        REPLY = 0b10u,
        ACK_NACK = 0b11u
    };

    uint8_t preamble;
    uint8_t source;
    uint8_t destination;
    uint32_t crc;

    struct
    {
        struct
        {
            struct
            {
                uint8_t length; /* Length of the payload field in bytes. */
            } msb;

            union
            {
                struct
                {
                    uint8_t type : 2; /* Message type. */
                    uint8_t nesn : 1; /* Next Expected
                                        Sequence Number:
                                        used by sender. */
                    uint8_t sn : 1;   /* Sequence Number: compared
                                        to NESN. If different,
                                        ACK. If not, NACK ->
                                        resend message. */
                    uint8_t md : 1;   /* More Data: 0 = finished,
                                        1 = more data is coming.
                                        Used for segmented
                                        messages. */
                    uint8_t rfu : 3;  /* Reserved for Future
                                         Use. */
                } lsb;
                uint8_t lsb_raw;
            };
        } header;
        uint8_t payload[PACKET_MAX_PAYLOAD_LENGTH] = {0};
    } pdu;

    /**
     * Default constructor, initialise everything to 0.
     */
    Packet_t(void);

    /**
     * Constructor.
     *
     * @param destination Destination.
     * @param length      Payload length.
     * @param type        Packet type.
     * @param payload     Optional pointer to message payload.
     */
    Packet_t(subsystems::Subsystem_t destination, uint8_t length, Packet_type_t type, uint8_t *payload = nullptr);

    /**
     * Transform a packet into an array of bytes.
     *
     * @param bytes Byte array to store the data in.
     */
    void to_bytes(uint8_t bytes[PACKET_MAX_PAYLOAD_LENGTH]);

    /**
     * Transform raw bytes into a packet.
     * Stores the information inside the packet itself.
     *
     * @param bytes Bytes to transform into a packet.
     */
    void from_bytes(uint8_t bytes[PACKET_MAX_PAYLOAD_LENGTH]);

    /**
     * Calculate the 24-bit CRC over an entire packet (excluding the CRC
     * itself). The CRC is stored inside the crc field of the packet.
     *
     * Calculation checked using https://leventozturk.com/engineering/crc/
     *
     * @param packet Packet to calculate CRC from.
     */
    void calculate_crc(uint8_t packet_raw[PACKET_MAX_PAYLOAD_LENGTH], size_t length = 0u);

    /**
     * Set all packet fields to 0.
     */
    void clear_all_fields(void);
};
