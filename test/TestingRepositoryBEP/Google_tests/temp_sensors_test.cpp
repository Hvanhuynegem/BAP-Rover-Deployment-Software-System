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

#include <gtest/gtest.h>
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

