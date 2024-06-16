//
// Created by henri on 16/06/2024.
//
/*
 * convert_array_to_message_tests.cpp file
 *
 * Testing file for the `convert_array_to_message` method. Multiple tests are executed here to demonstrate that the method behaves as expected.
 * Created by Henri Vanhuynegem on 07/06/2024.
 * Last edited: 10/06/2024.
 *
 * Tests:
 * - Normal test: Verify a normal array is correctly converted to a message.
 * - Length smaller than five: Handle an array that is smaller than length 5.
 * - Start byte test: Ensure the start byte is correctly placed in the message.
 * - Message type test: Verify the message type is correctly placed in the message.
 * - Message length test: Ensure the message length is correctly placed in the message.
 * - Message length longer than max payload size: Ensure that the message length longer than payload size is handled.
 * - Message length smaller than zero: Handle the case where the message length is smaller than zero.
 * - Empty payload test: Handle conversion when the payload is empty.
 * - Checksum test: Verify the checksum is correctly placed in the message.
 * - End byte test: Ensure the end byte is correctly placed in the message.
 * - Checksum verification test: Check whether the checksum is correct.
 */

#include "gtest/gtest.h"
#include "lander_communication.h"

TEST(convertArrayToMessageTestSuite, normalTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // expected message
    Message expected_msg;
    expected_msg.start_byte = 0x7E;
    expected_msg.msg_type = 0x01;
    expected_msg.length = 0x04;
    uint8_t expected_payload[] = {0x74, 0x65, 0x73, 0x74};
    memcpy(expected_msg.payload, expected_payload, expected_msg.length);
    expected_msg.checksum = 0x13;
    expected_msg.end_byte = 0x7F;

    // actual message
    Message actual_msg;
    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);

    EXPECT_EQ(expected_msg.start_byte, actual_msg.start_byte);
    EXPECT_EQ(expected_msg.msg_type, actual_msg.msg_type);
    EXPECT_EQ(expected_msg.length, actual_msg.length);
    EXPECT_EQ(0, memcmp(expected_msg.payload, actual_msg.payload, expected_msg.length));
    EXPECT_EQ(expected_msg.checksum, actual_msg.checksum);
    EXPECT_EQ(expected_msg.end_byte, actual_msg.end_byte);
}


TEST(convertArrayToMessageTestSuite, lengthSmallerThanFiveTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, 0x04};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // actual message
    Message actual_msg;
    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);

    EXPECT_FALSE(result);
}

TEST(convertArrayToMessageTestSuite, startByteTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // actual message
    Message actual_msg;
    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);

    EXPECT_TRUE(result);
    EXPECT_EQ(0x7E, actual_msg.start_byte);
}

TEST(convertArrayToMessageTestSuite, messageTypeTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // actual message
    Message actual_msg;
    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);

    EXPECT_TRUE(result);
    EXPECT_EQ(0x01, actual_msg.msg_type);
}

TEST(convertArrayToMessageTestSuite, messageLengthTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // actual message
    Message actual_msg;
    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);

    EXPECT_TRUE(result);
    EXPECT_EQ(0x04, actual_msg.length);
}

TEST(convertArrayToMessageTestSuite, messageLengthLongerThanMaxPayloadSizeTest) {
    // input buffer
    uint8_t input_buffer[MAX_PAYLOAD_SIZE + 10] = {0x7E, 0x01, MAX_PAYLOAD_SIZE + 1};
    uint16_t input_length = MAX_PAYLOAD_SIZE + 10;

    // actual message
    Message actual_msg;

    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);
    EXPECT_TRUE(result);
    EXPECT_EQ(actual_msg.length, 249);
}

TEST(convertArrayToMessageTestSuite, messageLengthSmallerThanZeroTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, (uint8_t)-1, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // actual message
    Message actual_msg;
    convert_array_to_message(input_buffer, input_length, &actual_msg);

    EXPECT_EQ(actual_msg.length, 249);
}

TEST(convertArrayToMessageTestSuite, emptyPayloadTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, 0x00, 0x01, 0x7F};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // expected message
    Message expected_msg;
    expected_msg.start_byte = 0x7E;
    expected_msg.msg_type = 0x01;
    expected_msg.length = 0x00;
    expected_msg.checksum = 0x01;
    expected_msg.end_byte = 0x7F;

    // actual message
    Message actual_msg;
    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);

    EXPECT_TRUE(result);
    EXPECT_EQ(expected_msg.start_byte, actual_msg.start_byte);
    EXPECT_EQ(expected_msg.msg_type, actual_msg.msg_type);
    EXPECT_EQ(expected_msg.length, actual_msg.length);
    EXPECT_EQ(expected_msg.checksum, actual_msg.checksum);
    EXPECT_EQ(expected_msg.end_byte, actual_msg.end_byte);
}

TEST(convertArrayToMessageTestSuite, checksumTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // actual message
    Message actual_msg;
    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);

    EXPECT_TRUE(result);
    EXPECT_EQ(0x13, actual_msg.checksum);
}

TEST(convertArrayToMessageTestSuite, endByteTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // actual message
    Message actual_msg;
    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);

    EXPECT_TRUE(result);
    EXPECT_EQ(0x7F, actual_msg.end_byte);
}

TEST(convertArrayToMessageTestSuite, checksumVerificationTest) {
    // input buffer
    uint8_t input_buffer[] = {0x7E, 0x01, 0x04, 0x74, 0x65, 0x73, 0x74, 0x13, 0x7F};
    uint16_t input_length = sizeof(input_buffer) / sizeof(input_buffer[0]);

    // actual message
    Message actual_msg;
    bool result = convert_array_to_message(input_buffer, input_length, &actual_msg);

    uint8_t calculated_checksum = calculate_checksum_helper(actual_msg.msg_type, actual_msg.length, actual_msg.payload);

    EXPECT_TRUE(result);
    EXPECT_EQ(calculated_checksum, actual_msg.checksum);
}

/*
 * convert_message_to_array_tests.cpp file
 *
 * Testing file for the `convert_message_to_array` method. Multiple tests are executed here to demonstrate that the method behaves as expected.
 * Created by Henri Vanhuynegem on 07/06/2024.
 * Last edited: 10/06/2024.
 *
 * Tests:
 * - Convert normal message to array test: Verify a normal message is correctly converted to an array.
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
 */


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

/*
 * supercap_readout_test.cpp file
 *
 * Testing file for ADC to voltage conversion function. Multiple tests are executed here to demonstrate that the method behaves as expected. Below is a list of all tested functionalities and situations.
 * Created by Henri Vanhuynegem on 16/06/2024.
 * Last edited: 16/06/2024.
 *
 * Tests for convert_adc_to_voltage function:
 * - Zero value test: This test checks whether the function correctly converts an ADC value of 0 to 0.0 V.
 * - Max value test: This test checks whether the function correctly converts the maximum ADC value (4095) to the maximum voltage (3.64 V).
 * - Mid value test: This test verifies the conversion when the ADC value is half of the maximum value.
 * - Quarter value test: This test verifies the conversion when the ADC value is a quarter of the maximum value.
 * - Three-quarters value test: This test verifies the conversion when the ADC value is three-quarters of the maximum value.
 * - Arbitrary value test: This test verifies the conversion for an arbitrary ADC value (2048).
 */

#include "supercap_readout.h"

// Constants
#define ADC_MAX_VALUE        4095      // 12-bit ADC resolution (2^12 - 1)
#define MAX_VOLTAGE          3.64      // Reference voltage for ADC (measured 3.64 V)



// Test cases for convert_adc_to_voltage function
TEST(AdcConversionTest, ZeroValue) {
    volatile unsigned int adc_value = 0;
    float expected_voltage = 0.0;
    EXPECT_FLOAT_EQ(convert_adc_to_voltage(adc_value), expected_voltage);
}

TEST(AdcConversionTest, MaxValue) {
    volatile unsigned int adc_value = ADC_MAX_VALUE;
    float expected_voltage = MAX_VOLTAGE;
    EXPECT_FLOAT_EQ(convert_adc_to_voltage(adc_value), expected_voltage);
}

TEST(AdcConversionTest, MidValue) {
    volatile unsigned int adc_value = ADC_MAX_VALUE / 2;
    float expected_voltage = MAX_VOLTAGE / 2;
    EXPECT_NEAR(convert_adc_to_voltage(adc_value), expected_voltage, 0.01);

}

TEST(AdcConversionTest, QuarterValue) {
    volatile unsigned int adc_value = ADC_MAX_VALUE / 4;
    float expected_voltage = MAX_VOLTAGE / 4;
    EXPECT_NEAR(convert_adc_to_voltage(adc_value), expected_voltage, 0.01);
}

TEST(AdcConversionTest, ThreeQuartersValue) {
    volatile unsigned int adc_value = 3 * (ADC_MAX_VALUE / 4);
    float expected_voltage = 3 * (MAX_VOLTAGE / 4);
    EXPECT_NEAR(convert_adc_to_voltage(adc_value), expected_voltage, 0.01);
}

TEST(AdcConversionTest, ArbitraryValue) {
    volatile unsigned int adc_value = 2048; // Arbitrary value
    float expected_voltage = (2048 * (float)MAX_VOLTAGE) / (float)ADC_MAX_VALUE;
    EXPECT_FLOAT_EQ(convert_adc_to_voltage(adc_value), expected_voltage);
}

/*
 * temp_sensors_test.cpp file
 *
 * Testing file for frequency calculation method and frequency to temperature conversion. Multiple tests are executed here to demonstrate that the methods behave as expected. Below is a list of all tested functionalities and situations.
 * Created by Henri Vanhuynegem on 16/06/2024.
 * Last edited: 16/06/2024.
 *
 * calculateFrequency Tests:
 * - Zero period test: This test checks whether the function returns zero if the period is zero.
 * - Positive period test: This test checks whether the function correctly calculates the frequency for a given positive period.
 * - Small period test: This test verifies the function with a non-zero, small period.
 *
 * frequency_to_temperature Tests:
 * - Zero frequency test: This test ensures that a default error value is returned if the frequency is zero.
 * - Positive frequency test: This test verifies the temperature conversion for a given positive frequency.
 * - Small frequency test: This test verifies the function with a small frequency.
 * - Frequency in range test: This test checks the temperature conversion for a frequency within the expected range.
 * - Frequency in range test 2: This test checks the temperature conversion for another frequency within the expected range.
 * - Frequency in range test 3: This test checks the temperature conversion for yet another frequency within the expected range.
 */


#include "temp_sensors.h"

// Test cases for calculateFrequency function
TEST(FrequencyCalculationTest, ZeroPeriod) {
    float period = 0.0;
    float expected_frequency = 0.0;
    EXPECT_FLOAT_EQ(calculateFrequency(period), expected_frequency);
}

TEST(FrequencyCalculationTest, PositivePeriod) {
    float period = 2.0;
    float expected_frequency = 2000000.0;
    EXPECT_FLOAT_EQ(calculateFrequency(period), expected_frequency);
}

TEST(FrequencyCalculationTest, SmallPeriod) {
    float period = 12;
    float expected_frequency = 333333.333333;
    EXPECT_FLOAT_EQ(calculateFrequency(period), expected_frequency);
}

// Test cases for frequency_to_temperature function
TEST(FrequencyToTemperatureTest, ZeroFrequency) {
    float frequency = 0.0;
    float expected_temperature = -99.0; // result from (1/0 - 1000) / 3.85, where 1/0 is theoretically infinite
    EXPECT_FLOAT_EQ(frequency_to_temperature(frequency), expected_temperature);
}

TEST(FrequencyToTemperatureTest, PositiveFrequency) {
    float frequency = 2000000.0;
    float expected_temperature = -99.0;
    EXPECT_FLOAT_EQ(frequency_to_temperature(frequency), expected_temperature);
}

TEST(FrequencyToTemperatureTest, SmallFrequency) {
    float frequency = 1.0;
    float expected_temperature = -99.0;
    EXPECT_FLOAT_EQ(frequency_to_temperature(frequency), expected_temperature);
}

TEST(FrequencyToTemperatureTest, FrequencyInRange) {
    float frequency = 150;
    float expected_temperature = 710.77972;
    EXPECT_FLOAT_EQ(frequency_to_temperature(frequency), expected_temperature);
}

TEST(FrequencyToTemperatureTest, FrequencyInRange2) {
    float frequency = 600;
    float expected_temperature = -17.110262;
    EXPECT_FLOAT_EQ(frequency_to_temperature(frequency), expected_temperature);
}

TEST(FrequencyToTemperatureTest, FrequencyInRange3) {
    float frequency = 800;
    float expected_temperature = -77.767761;
    EXPECT_FLOAT_EQ(frequency_to_temperature(frequency), expected_temperature);
}


