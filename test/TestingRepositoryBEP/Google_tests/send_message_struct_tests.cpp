/*
 * send_message_struct_tests.cpp file
 *
 * Testing file for the `send_message_struct` method. Multiple tests are executed here to demonstrate that the method behaves as expected.
 * Created by Henri Vanhuynegem on 10/06/2024.
 * Last edited: 10/06/2024.
 *
 * Tests:
 * - Normal test: Verify a normal array is correctly converted to a message.
 */

#include "gtest/gtest.h"
#include "lander_communication.h"

TEST(sendMessageStructTestSuite, sendMessageNormalTest){
    // create message structure
    Message msg;
    msg.start_byte = MSG_START_BYTE;
    msg.msg_type = MSG_TYPE_INIT;
    uint8_t input_payload[MAX_PAYLOAD_SIZE] = "test";
    msg.length = strlen((char *)input_payload);
    memcpy(msg.payload, input_payload, msg.length);
    msg.checksum = calculate_checksum_helper(msg.msg_type, msg.length, msg.payload);
    msg.end_byte = MSG_END_BYTE;

    // convert message to array
    send_message_struct(&msg);
}