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