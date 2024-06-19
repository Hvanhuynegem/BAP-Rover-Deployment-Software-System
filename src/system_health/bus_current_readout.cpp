/*
 * umbilical_cord.cpp file
 *
 * This file includes the functionalities for checking the bus current.
 *
 * Author: Henri Vanhuynegem
 * created: 18/06/2024
 * Last edited: 18/06/2024
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_health_lib/bus_current_readout.h"


// Function to initialize pin 4.3 as an input pin
void initialize_bus_current_sense_pin(void) {
    // Configure GPIO
    P4DIR &= ~BIT3;                         // Set pin 4.3 as input (bus current sensing pin)
    P4SEL1 |= BIT3;               // Select P4.3 for ADC function
    P4SEL0 |= BIT3;               // Select P4.3 for ADC function
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to initialize the ADC for bus current sense
void initialize_adc_bus_sense(void) {
    ADC12CTL0 &= ~ADC12ENC;            // Disable ADC12
    ADC12MCTL1 |= ADC12INCH_11;         // Select channel A7 for conversion
    ADC12IER0 |= ADC12IE1;             // Enable ADC conversion complete interrupt
    ADC12CTL0 |= ADC12ENC;             // Enable conversions
    PM5CTL0 &= ~LOCKLPM5;              // Disable the GPIO power-on default high-impedance mode
}




// Function to initialize pin 4.2 as an output pin
void initialize_bus_flag_pin(void) {
    // Configure GPIO
    P4DIR |= BIT2;                         // Set pin 4.2 as output (bus flag pin)
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}


float voltage_adc_bus_sense(void) {
    initialize_bus_current_sense_pin();             // Initialize the bus sense pin
    initialize_adc_bus_sense();        // Initialize the ADC for bus sense

    volatile unsigned int ADC_capture = 0; // Variable to store ADC result
    float voltage = 0;                    // Variable to store calculated voltage

    ADC12CTL0 |= ADC12SC;              // Start conversion - software trigger
    while (!(ADC12IFGR0 & BIT1) && !adc_conversion_fail); // Wait for conversion to complete or fail
    ADC_capture = ADC12MEM1;           // Read conversion result
    ADC12CTL0 &= ~ADC12SC;             // Ensure that it won't start a conversion again

    if (adc_conversion_fail) {
        // Conversion failed
        voltage = 99;                  // Set error voltage
        return voltage;
    } else {
        voltage = convert_adc_to_voltage(ADC_capture); // Convert ADC result to voltage
        return voltage;
    }
}

void float_to_uint8_array(float value, uint8_t* array, size_t n) {
    // Ensure the array is long enough to hold the float's byte representation
    if (n < sizeof(float)) {
        // Handle error, array is not long enough
        return;
    }

    // Copy the float's byte representation into the array
    memcpy(array, &value, sizeof(float));
}

