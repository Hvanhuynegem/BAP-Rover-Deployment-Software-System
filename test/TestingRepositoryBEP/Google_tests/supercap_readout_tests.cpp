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

#include <gtest/gtest.h>
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

