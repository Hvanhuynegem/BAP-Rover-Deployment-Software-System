/*
 * slip_decoding_tests.cpp file
 *
 * Testing file for SLIP decoding method. Multiple tests are executed here to demonstrate that the method behaves as expected. Below is a list of all tested functionalities and situations.
 * Created by Henri Vanhuynegem on 06/06/2024.
 * Last edited: 07/06/2024.
 *
 * Tests:
 * - Normal data test: Ensure regular data bytes are passed through unchanged after decoding.
 * - Start byte removed test: Verify the payload has the start byte removed.
 * - End byte removed test: Verify the payload has the end byte removed.
 * - Empty buffer test: Handle decoding when the input buffer is empty.
 * - Empty payload test: Handle decoding when the payload is empty.
 * - Input length larger than UART buffer test: Handle decoding when the input length exceeds the UART buffer size.
 * - ESC + ESC_END replacement test: Replace ESC + ESC_END bytes in the input buffer with END.
 * - ESC + ESC_ESC replacement test: Replace ESC + ESC_ESC bytes in the input buffer with ESC.
 * - Sequential END bytes test: Handle sequential END bytes correctly.
 * - Sequential ESC bytes test: Handle sequential ESC bytes correctly.
 * - Mixed ESC + ESC_END and ESC + ESC_ESC bytes test: Handle a mixture of ESC + ESC_END and ESC + ESC_ESC bytes correctly.
 * - Maximum buffer size test: Verify decoding with the maximum allowable input buffer size.
 * - Check all four different characters for decoding: Verify decoding for all special character sequences.
 * - Length zero test: Correctly return false if the input length is smaller than 2 or if the start or end byte is not equal to END.
 * - No first slip encoding character: returns false if there is no slip encoding character in the first position of the array
 * - No last slip encoding character: returns false if there is no slip encoding character in the last position of the array
 * - First ESC character and then no ESC_ESC OR ESC_END: ensure this returns false since this is invalid encoding.
 */

#include "gtest/gtest.h"
#include "lander_communication.h"

TEST(slip_decoding_testsSuite, NormalDataTest){
    // input buffer with data
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0x48, 0x69, 0xC0};
    uint16_t input_length = 4;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0x48, 0x69};
    uint16_t expected_length = 2;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, StartByteRemovedTest){
    // input buffer with data
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0x01, 0x02, 0xC0};
    uint16_t input_length = 4;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0x01, 0x02};
    uint16_t expected_length = 2;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, EndByteRemovedTest){
    // input buffer with data
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0x01, 0x02, 0xC0};
    uint16_t input_length = 4;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0x01, 0x02};
    uint16_t expected_length = 2;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, EmptyPayloadTest){
    // input buffer with no data between END bytes
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xC0};
    uint16_t input_length = 2;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {};
    uint16_t expected_length = 0;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
}

TEST(slip_decoding_testsSuite, InputLengthLargerThanUARTBufferTest){
    // input buffer filled with data for a length of 356
    uint8_t input_buffer[UART_BUFFER_SIZE + 100];
    memset(input_buffer, 1, sizeof(input_buffer));
    input_buffer[0] = 0xC0;
    input_buffer[355] = 0xC0;
    uint16_t input_length = 356;
    bool result;

    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool expected_result = false;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE];
    memset(expected_buffer, 1, UART_BUFFER_SIZE);
    uint16_t expected_length = UART_BUFFER_SIZE;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
}

TEST(slip_decoding_testsSuite, ESC_ENDReplacementTest){
    // input buffer with ESC + ESC_END sequence
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xC0};
    uint16_t input_length = 4;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0};
    uint16_t expected_length = 1;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, ESC_ESCReplacementTest){
    // input buffer with ESC + ESC_ESC sequence
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDD, 0xC0};
    uint16_t input_length = 4;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xDB};
    uint16_t expected_length = 1;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, SequentialENDBytesTest){
    // input buffer with sequential END bytes
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xDB, 0xDC, 0xC0};
    uint16_t input_length = 6;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0, 0xC0};
    uint16_t expected_length = 2;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, SequentialESCBytesTest){
    // input buffer with sequential ESC bytes
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDD, 0xDB, 0xDD, 0xC0};
    uint16_t input_length = 6;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xDB, 0xDB};
    uint16_t expected_length = 2;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, MixedESC_ENDAndESC_ESCBytesTest){
    // input buffer with mixed ESC + ESC_END and ESC + ESC_ESC sequences
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xDB, 0xDD, 0xDB, 0xDC, 0xDB, 0xDD, 0xC0};
    uint16_t input_length = 10;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xC0, 0xDB};
    uint16_t expected_length = 4;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, MaximumPayloadSizeTest){
    // input buffer with maximum data size
    uint8_t input_buffer[UART_BUFFER_SIZE];
    memset(input_buffer, 0x01, UART_BUFFER_SIZE);
    input_buffer[0] = 0xC0;
    input_buffer[UART_BUFFER_SIZE - 1] = 0xC0;
    uint16_t input_length = UART_BUFFER_SIZE;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected result
    uint8_t expected_buffer[UART_BUFFER_SIZE];
    memset(expected_buffer, 0x01, UART_BUFFER_SIZE - 2);
    uint16_t expected_length = UART_BUFFER_SIZE - 2;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, CheckAllFourSpecialCharacters) {
    // input buffer with all four special characters
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xDB, 0xDD, 0xDC, 0xDD, 0xC0};
    uint16_t input_length = 8;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xDD};
    uint16_t expected_length = 4;
    bool expected_result = true;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(slip_decoding_testsSuite, LengthZeroTest){
    // input buffer with length zero
    uint8_t input_buffer[UART_BUFFER_SIZE] = {};
    uint16_t input_length = 0;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected result
    bool expected_result = false;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
}

TEST(slip_decoding_testsSuite, StartOrEndByteNotENDTest){
    // input buffer with incorrect start or end byte
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0x01, 0x02, 0x03};
    uint16_t input_length = 3;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // expected result
    bool expected_result = false;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_EQ(result, expected_result);
}

TEST(slip_decoding_testsSuite, testFirstCharacter){
    // input buffer with all four special characters
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xDB, 0xDC, 0xDB, 0xDD, 0xDC, 0xDD, 0xC0};
    uint16_t input_length = 8;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_FALSE(result);
}

TEST(slip_decoding_testsSuite, testlastCharacter){
    // input buffer with all four special characters
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDC, 0xDB, 0xDD, 0xDC, 0xDD};
    uint16_t input_length = 8;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    EXPECT_FALSE(result);
}

TEST(slip_decoding_testsSuite, isEscapedTest){
    // input buffer with all four special characters
    uint8_t input_buffer[UART_BUFFER_SIZE] = {0xC0, 0xDB, 0xDD, 0xDB, 0x02, 0xC0};
    uint16_t input_length = 6;
    // output buffer after decoding
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint16_t output_length;
    bool result;

    // decoding
    result = slip_decode(input_buffer, input_length, output_buffer, &output_length);

    // Assertions
    // Assertions
    EXPECT_FALSE(result);
}

