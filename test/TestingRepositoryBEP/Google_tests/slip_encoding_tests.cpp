/*
 * slip_encoding_tests.cpp file
 *
 * Testing file for SLIP encoding method. Multiple tests are executed here to demonstrate that the method behaves as expected. Below is a list of all tested functionalities and situations.
 * Created by Henri Vanhuynegem on 06/06/2024.
 * Last edited: 07/06/2024.
 *
 * Tests:
 * - Normal data test: Ensure regular data bytes are passed through unchanged.
 * - Start byte test: Verify the output starts with the END byte.
 * - End byte test: Verify the output ends with the END byte.
 * - Empty buffer test: Handle encoding when the input buffer is empty.
 * - Buffer larger than UART buffer test: Handle encoding when the input buffer exceeds the UART buffer size.
 * - Buffer smaller than UART buffer but after encoding, it is larger: Handle encoding when input buffer expands and exceeds UART buffer size after encoding.
 * - END replacement test: Replace END bytes in the input buffer with ESC + ESC_END.
 * - ESC replacement test: Replace ESC bytes in the input buffer with ESC + ESC_ESC.
 * - Sequential END bytes test: Handle sequential END bytes correctly.
 * - Sequential ESC bytes test: Handle sequential ESC bytes correctly.
 * - Mixed END and ESC bytes test: Handle a mixture of END and ESC bytes correctly.
 * - Maximum payload size test: Verify encoding with the maximum allowable input buffer size.
 * - Buffer overflow test: Check for buffer overflow when encoding.
 * - ESC overflow test: Check for not enough space for the ESC and ESC_ESC sequence.
 * - Maximum encodable data test: Handle encoding expansion up to UART buffer size.
 * - Mixed special characters test: Check encoding of a mixture of special characters.
 */

#include "gtest/gtest.h"
#include "lander_communication.h"


TEST(slip_encoding_testsSuite, NormalDataTest){
    // input buffer with data
    uint8_t input_buffer[UART_BUFFER_SIZE] = "Hi";
    uint16_t input_length = 2;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xc0, 0x48, 0x69, 0xc0};
    uint16_t expected_length = 4;
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, 4);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_encoding_testsSuite, StartByteTest){
    // input buffer with data
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0x01,0x02};
    uint16_t input_length = 2;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected character
    uint8_t expected_character = 0xc0;
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_buffer[0], expected_character);

}

TEST(slip_encoding_testsSuite, EndByteTest){
    // input buffer with data
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0x01,0x02};
    uint16_t input_length = 2;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected character
    uint8_t expected_character = 0xc0;
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_buffer[3], expected_character);
}

TEST(slip_encoding_testsSuite, EmptyBufferTest){
    // input buffer with data
    uint8_t input_buffer[UART_BUFFER_SIZE] = {};
    uint16_t input_length = 0;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xc0, 0xc0};
    uint16_t expected_length = 2;

    // encoding
    slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(output_length, 2);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_encoding_testsSuite, BufferLargerThanUARTBufferTest){
    // input buffer filled with 1 for a length of 356
    uint8_t input_buffer[UART_BUFFER_SIZE + 100]; // 256 + 100
    memset(input_buffer, 1, sizeof(input_buffer));
    uint16_t input_length = 356;
    bool result;

    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool expected_result = false;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE];
    memset(expected_buffer, 1, UART_BUFFER_SIZE);
    expected_buffer[0] = 0xc0;
    expected_buffer[255] = 0xc0;
    uint16_t expected_length = 256;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
}

TEST(slip_encoding_testsSuite, BufferSmallerThanUARTBufferBeforeEncodingTest){
    // input buffer with data that will expand during encoding
    uint8_t input_buffer[UART_BUFFER_SIZE - 20]; // 256 + 100
    memset(input_buffer, 0xC0, sizeof(input_buffer));
    uint16_t input_length = UART_BUFFER_SIZE - 20;
    bool result;

    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;

    // expected result
    bool expected_result = false;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
}

TEST(slip_encoding_testsSuite, ENDReplacementTest){
    // input buffer with data containing END byte
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0};
    uint16_t input_length = 1;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xC0};
    uint16_t expected_length = 4;
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_encoding_testsSuite, ESCReplacementTest){
    // input buffer with data containing ESC byte
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xDB};
    uint16_t input_length = 1;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDD, 0xC0};
    uint16_t expected_length = 4;
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_encoding_testsSuite, SequentialENDBytesTest){
    // input buffer with sequential END bytes
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xC0};
    uint16_t input_length = 2;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xDB, 0xDC, 0xC0};
    uint16_t expected_length = 6;
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_encoding_testsSuite, SequentialESCBytesTest){
    // input buffer with sequential ESC bytes
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xDB, 0xDB};
    uint16_t input_length = 2;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDD, 0xDB, 0xDD, 0xC0};
    uint16_t expected_length = 6;
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_encoding_testsSuite, MixedENDandESCBytesTest){
    // input buffer with mixed END and ESC bytes
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xC0, 0xDB};
    uint16_t input_length = 4;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xDB, 0xDD, 0xDB, 0xDC, 0xDB, 0xDD, 0xC0};
    uint16_t expected_length = 10;
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_encoding_testsSuite, MaximumPayloadSizeTest){
    // input buffer with maximum data size
    uint8_t input_buffer[UART_BUFFER_SIZE];
    memset(input_buffer, 0x01, UART_BUFFER_SIZE - 2);
    uint16_t input_length = UART_BUFFER_SIZE - 2;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected result
    uint8_t expected_buffer[UART_BUFFER_SIZE];
    memset(expected_buffer, 0x01, UART_BUFFER_SIZE);
    expected_buffer[0] = 0xc0;
    expected_buffer[255] = 0xc0;
    uint16_t expected_length = UART_BUFFER_SIZE;
    bool expected_result = true; // Since encoding should not overflow

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_encoding_testsSuite, BufferOverflowTest) {
    // input buffer filled with 1 for a length equal to UART_BUFFER_SIZE
    uint8_t input_buffer[UART_BUFFER_SIZE];
    memset(input_buffer, 1, sizeof(input_buffer)-1);
    uint16_t input_length = UART_BUFFER_SIZE;
    bool result;

    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;

    // expected result
    bool expected_result = false;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
}

TEST(slip_encoding_testsSuite, EscOverflowTest) {
    // input buffer filled with ESC characters, causing overflow
    uint8_t input_buffer[UART_BUFFER_SIZE];
    for (uint16_t i = 0; i < UART_BUFFER_SIZE; i++) {
        input_buffer[i] = 0xDB; // ESC character
    }
    uint16_t input_length = UART_BUFFER_SIZE;
    bool result;

    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;

    // expected result
    bool expected_result = false;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
}

TEST(slip_encoding_testsSuite, MaximumEncodableDataTest) {
    // input buffer with data that would expand to just fit within UART_BUFFER_SIZE
    uint8_t input_buffer[UART_BUFFER_SIZE];
    memset(input_buffer, 0xC0, (UART_BUFFER_SIZE-2) / 2); // fill with END bytes
    uint16_t input_length = (UART_BUFFER_SIZE-2) / 2;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected result
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
}

TEST(slip_encoding_testsSuite, MixedSpecialCharactersTest) {
    // input buffer with mixed special characters
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xDD}; // END
    uint16_t input_length = 4;
    // output buffer after encoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xDB, 0xDD, 0xDC, 0xDD, 0xC0};
    uint16_t expected_length = 8;
    bool expected_result = true;

    // encoding
    result = slip_encode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

