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
#include <stdio.h>

#include "system_health_lib/bus_current_readout.h"


// Function to initialize pin 4.3 as an input pin
void initialize_bus_current_sense_pin(void) {
    // Configure GPIO
    P4DIR &= ~BIT3;                         // Set pin 4.3 as input (bus current sensing pin)
    P4SEL1 |= BIT3;                         // Select P4.3 for ADC function
    P4SEL0 |= BIT3;                         // Select P4.3 for ADC function
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to initialize the ADC for bus current sense
void initialize_adc_bus_sense(void) {
    // Turn on ADC12, set sampling time, and select clock source
    ADC12CTL0 &= ~ADC12ENC;               // Disable ADC before configuration
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;    // 16 ADC12CLK cycles, ADC on
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_0 | ADC12SSEL_3; // ADCCLK = SMCLK; single-channel single-conversion
    ADC12CTL2 = ADC12RES_2;               // 12-bit conversion results
    ADC12MCTL0 = ADC12INCH_11;            // A11 ADC input select; Vref=AVCC
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

    unsigned int adc_value = read_ADC();

    if (adc_value == 0xFFFF) {
        // Conversion failed
        return 99; // Set error voltage
    } else {
        return convert_adc_to_voltage(adc_value);// return the converted ADC result to voltage
    }
}

void float_to_uint8_array(float value, uint8_t* array, size_t n) {
    // Extract the integer and fractional parts
    int integer_part = (int)value;
    int decimal_part = (int)((value - integer_part) * 100); // Two decimal places

    // Handle negative values
    if (integer_part < 0) {
        integer_part = -integer_part;
        decimal_part = -decimal_part;
    }

    // Manually format the float into the array as "X.XX"
    array[0] = '0' + integer_part;       // Convert integer part to character
    array[1] = '.';                      // Decimal point
    array[2] = '0' + (decimal_part / 10); // First decimal place
    array[3] = '0' + (decimal_part % 10); // Second decimal place
    array[4] = '\0';                     // Null terminator
}