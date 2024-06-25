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

//// Function to initialize the ADC for bus current sense for polling
//void initialize_adc_bus_sense(void) {
//    // Turn on ADC12, set sampling time, and select clock source
//    ADC12CTL0 &= ~ADC12ENC;               // Disable ADC before configuration
//    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;    // 16 ADC12CLK cycles, ADC on
//    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_0 | ADC12SSEL_3; // ADCCLK = SMCLK; single-channel single-conversion
//    ADC12CTL2 = ADC12RES_2;               // 12-bit conversion results
//    ADC12MCTL0 = ADC12INCH_11;            // A11 ADC input select; Vref=AVCC
//}

void initialize_adc_bus_current(void) {
    ADC12CTL0 &= ~ADC12ENC;            // Disable ADC12
    ADC12MCTL0 |= ADC12INCH_11;         // Select channel A11 for conversion
    ADC12CTL0 |= ADC12ENC;             // Enable conversions
    PM5CTL0 &= ~LOCKLPM5;              // Disable the GPIO power-on default high-impedance mode
}


// Function to initialize pin 4.2 as an output pin
void initialize_bus_flag_pin(void) {
    // Configure GPIO
    P4DIR |= BIT2;                         // Set pin 4.2 as output (bus flag pin)
    P4OUT &= ~BIT2;                           // switch on power to rover
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

void switch_on_bus_flag_pin(void){
    P4OUT &= ~BIT2;                           // switch on power to rover (active low)
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

void switch_off_bus_flag_pin(void){
    P4OUT |= BIT2;                           // switch off power to rover (active low)
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// for polling
//float voltage_adc_bus_sense(void) {
//    initialize_bus_current_sense_pin();             // Initialize the bus sense pin
//    initialize_adc_bus_sense();        // Initialize the ADC for bus sense
//
//    unsigned int adc_value = read_ADC();
//
//    if (adc_value == 0xFFFF) {
//        // Conversion failed
//        return 99; // Set error voltage
//    } else {
//        return convert_adc_to_voltage(adc_value);// return the converted ADC result to voltage
//    }
//}

// Function to get the voltage of bus current via ADC
float voltage_adc_bus_sense(void) {
    initialize_adc_bus_current();      // Initialize the ADC for bus current sensing
    float voltage = 0;                 // Variable to store calculated voltage

    enable_interrupt_adc();
    ADC12CTL0 |= ADC12SC;              // Start conversion - software trigger
    while (!measurement_finished && !adc_conversion_fail);
    disable_interrupt_adc();
    measurement_finished = false;
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

#include <stdint.h>
#include <stdbool.h>

void float_to_uint8_array_2(float value, uint8_t* array) {
    // Extract the integer and fractional parts
    int integer_part = (int)value;
    int decimal_part = (int)((value - integer_part) * 10000); // Four decimal places

    // Handle negative values
    bool is_negative = false;
    if (value < 0) {
        is_negative = true;
        integer_part = -integer_part;
        decimal_part = -decimal_part;
    }

    // Manually format the float into the array as "XXX.XXXX"
    if (is_negative) {
        array[0] = '-';
        array[1] = '0' + ((integer_part / 100) % 10); // Hundreds digit of integer part
    } else {
        array[0] = '0' + ((integer_part / 100) % 10); // Hundreds digit of integer part
    }
    array[1] = '0' + ((integer_part / 10) % 10);  // Tens digit of integer part
    array[2] = '0' + (integer_part % 10);         // Units digit of integer part
    array[3] = '.';                               // Decimal point
    array[4] = '0' + ((decimal_part / 1000) % 10); // First decimal place
    array[5] = '0' + ((decimal_part / 100) % 10);  // Second decimal place
    array[6] = '0' + ((decimal_part / 10) % 10);   // Third decimal place
    array[7] = '0' + (decimal_part % 10);          // Fourth decimal place
    array[8] = '\0';                              // Null terminator
}

