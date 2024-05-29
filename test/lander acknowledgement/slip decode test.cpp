#include <stdint.h>
#include <stdio.h>

void slip_decode(const uint8_t *input_buffer, uint16_t input_length, uint8_t *output_buffer, uint16_t *output_length) {
    const uint8_t END = 0xC0;
    const uint8_t ESC = 0xDB;
    const uint8_t ESC_END = 0xDC;
    const uint8_t ESC_ESC = 0xDD;

    bool is_escaped = false;
    *output_length = 0;

    for (uint16_t i = 0; i < input_length; i++) {
        uint8_t c = input_buffer[i];

        if (c == END) {
            if (*output_length > 0) {
                // End of packet
                return;
            } else {
                // Start of packet, skip
                continue;
            }
        }

        if (is_escaped) {
            if (c == ESC_END) {
                c = END;
            } else if (c == ESC_ESC) {
                c = ESC;
            }
            is_escaped = false;
        } else if (c == ESC) {
            is_escaped = true;
            continue;
        }

        output_buffer[(*output_length)++] = c;
    }
}

void print_buffer(const uint8_t *buffer, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        printf("0x%02X ", buffer[i]);
    }
    printf("\n");
}

int main(void) {
    uint8_t payload[] = "\xC0~\x01\x16Initialization payload\x17\x7F\xC0";
    uint8_t decoded_msg[256];
    uint16_t input_length = sizeof(payload) - 1;  // exclude the null terminator
    uint16_t decoded_length;

    slip_decode(payload, input_length, decoded_msg, &decoded_length);

    printf("Decoded message: ");
    print_buffer(decoded_msg, decoded_length);

    return 0;
}
