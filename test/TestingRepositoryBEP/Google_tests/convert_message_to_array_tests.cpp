/*
 * convert_message_to_array_tests.cpp file
 *
 * Testing file for the `convert_message_to_array` method. Multiple tests are executed here to demonstrate that the method behaves as expected.
 * Created by Henri Vanhuynegem on 07/06/2024.
 * Last edited: 10/06/2024.
 *
 * Tests:
 * - Normal test: Verify a normal message is correctly converted to an array.
 * - Start byte test: Ensure the start byte is correctly placed in the output array.
 * - Message type test: Verify the message type is correctly placed in the output array.
 * - Message length test: Ensure the message length is correctly placed in the output array.
 * - Empty payload test: Handle conversion when the payload is empty.
 * - Payload longer than UART buffer test: Handle conversion when the payload length exceeds the UART buffer size.
 * - Checksum test: Verify the checksum is correctly calculated and placed in the output array.
 * - End byte test: Ensure the end byte is correctly placed in the output array.
 * - Buffer check test: Verify the entire buffer is correctly populated with expected values.
 * - Length check test: Ensure the output length is correctly calculated and returned.
 */

#include "gtest/gtest.h"
#include "lander_communication.h"

TEST(convertMessageToArrayTestSuite, convertNormalMessageToArrayTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "test";
    msg.length = strlen((char *)input_payload);
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected result
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0x7E, 0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    uint8_t expected_length = 9;

    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(convertMessageToArrayTestSuite, startByteTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "test";
    msg.length = strlen((char *)input_payload);
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected start byte
    EXPECT_EQ(output_buffer[0], MSG_START_BYTE);
}

TEST(convertMessageToArrayTestSuite, messageTypeTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "test";
    msg.length = strlen((char *)input_payload);
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected message type
    EXPECT_EQ(output_buffer[1], MSG_TYPE_INIT);
}

TEST(convertMessageToArrayTestSuite, messageLengthTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "test";
    msg.length = strlen((char *)input_payload);
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected length
    EXPECT_EQ(output_buffer[2], msg.length);
}

TEST(convertMessageToArrayTestSuite, emptyPayloadTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "";
    msg.length = 0;
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected result
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0x7E, 0x01, 0x00, 0x01, 0x7F}; // Checksum would be only msg_type + length
    uint8_t expected_length = 5;

    EXPECT_EQ(output_length, expected_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(convertMessageToArrayTestSuite, payloadLongerThanUARTBufferTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE]; // longer than UART_BUFFER_SIZE
    memset(input_payload, 'A', MAX_PAYLOAD_SIZE);
    msg.length = UART_BUFFER_SIZE-1;
    memcpy(msg.payload, input_payload, MAX_PAYLOAD_SIZE);
    msg.checksum = calculate_checksum_helper(msg.msg_type, MAX_PAYLOAD_SIZE, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected result
    uint8_t expected_length = UART_BUFFER_SIZE-2;
    uint8_t expected_buffer[UART_BUFFER_SIZE]; // longer than UART_BUFFER_SIZE
    memset(expected_buffer, 'A', expected_length);
    expected_buffer[0] = MSG_START_BYTE;
    expected_buffer[1] = MSG_TYPE_INIT;
    expected_buffer[2] = MAX_PAYLOAD_SIZE;
    expected_buffer[UART_BUFFER_SIZE-4] = 0xb9;
    expected_buffer[UART_BUFFER_SIZE-3] = MSG_END_BYTE;

    EXPECT_EQ(expected_length, output_length);
    for(int i = 0; i < expected_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(convertMessageToArrayTestSuite, checksumTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "test";
    msg.length = strlen((char *)input_payload);
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected checksum
    uint8_t expected_checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    EXPECT_EQ(output_buffer[7], expected_checksum);
}

TEST(convertMessageToArrayTestSuite, endByteTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "test";
    msg.length = strlen((char *)input_payload);
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected end byte
    EXPECT_EQ(output_buffer[8], MSG_END_BYTE);
}

TEST(convertMessageToArrayTestSuite, bufferCheckTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "test";
    msg.length = strlen((char *)input_payload);
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected buffer
    uint8_t expected_buffer[UART_BUFFER_SIZE] = {0x7E, 0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    for(int i = 0; i < output_length; i++){
        EXPECT_EQ(output_buffer[i], expected_buffer[i]);
    }
}

TEST(convertMessageToArrayTestSuite, lengthCheckTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "test";
    msg.length = strlen((char *)input_payload);
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // output buffers
    uint8_t output_buffer[UART_BUFFER_SIZE];
    uint8_t output_length;

    // convert message to array
    convert_message_to_array(&msg, output_buffer, &output_length);

    // expected length
    uint8_t expected_length = 9;
    EXPECT_EQ(output_length, expected_length);
}
