/*
 * supercap_readout.cpp
 *
 * This file includes the functions needed to initialise capacitor pins
 *
 *
 * Author: Diederik Aris
 * created: 28/05/2024
 * Last edited: 15/06/2024
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_health_lib/supercap_readout.h"

// Constants
#define ADC_MAX_VALUE        4095      // 12-bit ADC resolution (2^12 - 1)
#define MAX_VOLTAGE          3.64      // Reference voltage for ADC (measured 3.64 V)

// Global flag for ADC conversion failure
volatile bool adc_conversion_fail = false;

// ADC12 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
  {
    case ADC12IV_NONE:
        // No interrupt
        break;
    case ADC12IV_ADC12OVIFG:
        // ADC12MEMx Overflow (could be implemented later)
        break;
    case ADC12IV_ADC12TOVIFG:
        // Conversion time overflow
        adc_conversion_fail = true;
        ADC12CTL0 &= ~ADC12SC; // Stop conversion
        break;
    default:
        break;
  }
}

// Function to initialize the Super cap charge cap flags and cap discharge flag
void initialize_charge_cap_flags(void) {
    // charge cap flag 1
    P2DIR |= BIT7;               // Set P2.7 as output
    P2OUT &= ~BIT7;               // sets P2.7 to low (active high pin)

    // charge cap flag 2
    P2DIR |= BIT3;               // Set P2.3 as output
    P2OUT &= ~BIT3;               // sets P2.3 to low (active high pin)

    // charge cap flag 3
    P4DIR |= BIT4;               // Set P4.4 as output
    P4OUT &= ~BIT4;               // sets P4.4 to low (active high pin)

    // discharge cap flag
    PJDIR |= BIT4;               // Set Pj.4 as output
    PJOUT &= ~BIT4;               // sets Pj.4 to low (active high pin)

    // Disable the GPIO power-on default high-impedance mode
    PM5CTL0 &= ~LOCKLPM5;
}


// Function to initialize the Super cap voltage pin
void initialize_capready(void) {
    P2DIR &= ~BIT4;               // Set P2.4 as input
    P2SEL1 |= BIT4;               // Select P2.4 for special function
    P2SEL0 |= BIT4;               // Select P2.4 for special function
    PM5CTL0 &= ~LOCKLPM5;         // Disable the GPIO power-on default high-impedance mode
}


// Function to initialize the ADC for super capacitors
void initialize_adc_supercaps(void) {
    ADC12CTL0 &= ~ADC12ENC;            // Disable ADC12
    ADC12MCTL0 |= ADC12INCH_7;         // Select channel A7 for conversion
    ADC12IER0 |= ADC12IE0;             // Enable ADC conversion complete interrupt
    ADC12CTL0 |= ADC12ENC;             // Enable conversions
    PM5CTL0 &= ~LOCKLPM5;              // Disable the GPIO power-on default high-impedance mode
}

// Function to convert ADC value to voltage
float convert_adc_to_voltage(volatile unsigned int adc_value) {
    return (adc_value * (float)MAX_VOLTAGE) / (float)ADC_MAX_VALUE;
}

// Function to get the voltage of super capacitors via ADC
float voltage_adc_supercaps(void) {
    initialize_capready();             // Initialize the super cap voltage pin
    initialize_adc_supercaps();        // Initialize the ADC for super capacitors

    volatile unsigned int ADC_capture = 0; // Variable to store ADC result
    float voltage = 0;                    // Variable to store calculated voltage

    ADC12CTL0 |= ADC12SC;              // Start conversion - software trigger
    while (!(ADC12IFGR0 & BIT0) && !adc_conversion_fail); // Wait for conversion to complete or fail
    ADC_capture = ADC12MEM0;           // Read conversion result
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

