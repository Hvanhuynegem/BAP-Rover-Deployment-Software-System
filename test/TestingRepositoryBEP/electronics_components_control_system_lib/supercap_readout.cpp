//
// Created by henri on 16/06/2024.
//

#include "supercap_readout.h"

// Constants
#define ADC_MAX_VALUE        4095      // 12-bit ADC resolution (2^12 - 1)
#define MAX_VOLTAGE          3.64      // Reference voltage for ADC (measured 3.64 V)

// Function to convert ADC value to voltage
float convert_adc_to_voltage(volatile unsigned int adc_value) {
    return (adc_value * (float)MAX_VOLTAGE) / (float)ADC_MAX_VALUE;
}