#include <cstring>

#include "packet.h"

Packet_t::Packet_t() = default;

Packet_t::Packet_t(subsystems::Subsystem_t destination, uint8_t length, Packet_type_t type, uint8_t *payload) : destination(destination)
{
    pdu.header.lsb_raw = 0;
    preamble = PACKET_PREAMBLE;
    pdu.header.msb.length = length;
    pdu.header.lsb.type = type;

    if(payload != nullptr)
    {
        memcpy(pdu.payload, payload, length);
    }
}

void Packet_t::to_bytes(uint8_t bytes[PACKET_MAX_LENGTH])
{
    uint8_t payload_length = this->pdu.header.msb.length;

    /* Copy preamble, source, destination and header. */
    bytes[0] = PACKET_PREAMBLE;
    bytes[1] = this->source;
    bytes[2] = this->destination;
    bytes[3] = this->pdu.header.lsb_raw;
    bytes[4] = payload_length;

    /* Copy data. */
    memcpy(&bytes[PACKET_HEAD_LENGTH], this->pdu.payload, payload_length);

    /* Copy CRC. */
    bytes[payload_length + PACKET_HEAD_LENGTH] = (this->crc >> 16u) & 0xFFu;
    bytes[payload_length + PACKET_HEAD_LENGTH + 1u] = (this->crc >> 8u) & 0xFFu;
    bytes[payload_length + PACKET_HEAD_LENGTH + 2u] = (this->crc) & 0xFFu;
}

void Packet_t::from_bytes(uint8_t bytes[PACKET_MAX_LENGTH])
{
    uint8_t payload_length = bytes[4];

    /* Fill the preamble. */
    this->preamble = bytes[0];

    /* Fill the addressing information. */
    this->source = bytes[1];
    this->destination = bytes[2];

    /* Fill the header (first two bytes of PDU). */
    this->pdu.header.lsb_raw = bytes[3];
    this->pdu.header.msb.length = payload_length;

    /* Copy payload from raw data into the packet. */
    memcpy(this->pdu.payload, &bytes[5], payload_length);

    /* Fill in the CRC. */
    this->crc = (static_cast<uint32_t>(bytes[payload_length + PACKET_HEAD_LENGTH]) << 16u) & 0xFF0000u;
    this->crc |= (bytes[payload_length + PACKET_HEAD_LENGTH + 1u] << 8u) & 0x00FF00u;
    this->crc |= (bytes[payload_length + PACKET_HEAD_LENGTH + 2u] << 0u) & 0x0000FFu;
}

void Packet_t::calculate_crc(uint8_t packet_raw[PACKET_MAX_LENGTH], size_t length)
{
    /* CRC preset, see Volume 5, Section 3.1.1 of Bluetooth 4.2 specification. */
    uint32_t new_crc = 0x555555u;
    /* CRC polynomial, see Volume 5, Section 3.1.1 of Bluetooth 4.2 specification. */
    constexpr uint32_t polynomial = 0b11001011011u;
    /* Length used in CRC calculation. */
    size_t crc_packet_length = length ? (length - PACKET_CRC_LENGTH) : (PACKET_HEAD_LENGTH + this->pdu.header.msb.length);
    uint32_t bit;

    /* Convert packet to raw bytes. */
    this->to_bytes(packet_raw);

    /* Loop over payload bytes LSB first. */
    for(uint8_t index = 0u; index < crc_packet_length; index++)
    {
        /* Loop through bits in the XOR'ed byte. */
        for(uint8_t bit_index = 0; bit_index < 8u; bit_index++)
        {
            bit = (packet_raw[index] >> (7u - bit_index)) & 1u;

            /* For each added bit, shift CRC to the left. XOR with polynomial if MSB is 1. */
            new_crc = ((new_crc ^ (bit << 23u)) & 0x800000u) ? ((new_crc << 1u) ^ polynomial) : (new_crc << 1u);
        }
    }

    /* Only return 24 bits. */
    this->crc = new_crc & 0xFFFFFFu;
}

void Packet_t::clear_all_fields()
{
    /* Clear header. */
    this->pdu.header.lsb_raw = 0u;
    this->pdu.header.msb.length = 0u;

    /* Clear payload. */
    memset(this->pdu.payload, 0, PACKET_MAX_PAYLOAD_LENGTH);
}
