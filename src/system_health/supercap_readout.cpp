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

bool supercap_functionality[3]  = {false,false,false};

// Constants
#define ADC_MAX_VALUE        4095      // 12-bit ADC resolution (2^12 - 1)
#define MAX_VOLTAGE          3.64      // Reference voltage for ADC (measured 3.64 V)
#define ADC_TIMEOUT 100 // Define a timeout value

//// Global flag for ADC conversion failure
//volatile bool adc_conversion_fail = false;
//
//// ADC12 interrupt service routine
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector = ADC12_VECTOR
//__interrupt void ADC12_ISR(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
//#else
//#error Compiler not supported!
//#endif
//{
//  switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
//  {
//    case ADC12IV_NONE:
//        // No interrupt
//        break;
//    case ADC12IV_ADC12OVIFG:
//        // ADC12MEMx Overflow (could be implemented later)
//        break;
//    case ADC12IV_ADC12TOVIFG:
//        // Conversion time overflow
//        adc_conversion_fail = true;
//        ADC12CTL0 &= ~ADC12SC; // Stop conversion
//        ADC12CTL0 &= ~ADC12ENC; // Disable ADC to clear interrupt flag
//        ADC12CTL0 |= ADC12ENC; // Re-enable ADC
//        break;
//    default:
//        break;
//  }
//}

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

// Function to switch on a specific charge cap flag
void switch_on_charge_cap_flag(int flag) {
    switch (flag) {
        case 0:
            P2OUT |= BIT7;       // sets P2.7 to high
            break;
        case 1:
            P2OUT |= BIT3;       // sets P2.3 to high
            break;
        case 2:
            P4OUT |= BIT4;       // sets P4.4 to high
            break;
        default:
            // Handle invalid flag
            break;
    }
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to switch off a specific charge cap flag
void switch_off_charge_cap_flag(int flag) {
    switch (flag) {
        case 0:
            P2OUT &= ~BIT7;      // sets P2.7 to low
            break;
        case 1:
            P2OUT &= ~BIT3;      // sets P2.3 to low
            break;
        case 2:
            P4OUT &= ~BIT4;      // sets P4.4 to low
            break;
        default:
            // Handle invalid flag
            break;
    }
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Functions to switch on/off discharge cap flag
void switch_on_discharge_cap_flag(void) {
    PJOUT |= BIT4;               // sets PJ.4 to high
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

void switch_off_discharge_cap_flag(void) {
    PJOUT &= ~BIT4;              // sets PJ.4 to low
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
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
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON; // 16 ADC12CLK cycles, ADC on
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_0 | ADC12SSEL_3; // ADCCLK = SMCLK; single-channel single-conversion
    ADC12CTL2 = ADC12RES_2;            // 12-bit conversion results
    ADC12MCTL0 = ADC12INCH_7;          // Select channel A7 for conversion
    PM5CTL0 &= ~LOCKLPM5;              // Disable the GPIO power-on default high-impedance mode
}

// Function to convert ADC value to voltage
float convert_adc_to_voltage(volatile unsigned int adc_value) {
    return (adc_value * (float)MAX_VOLTAGE) / (float)ADC_MAX_VALUE;
}

// Function to read ADC value with polling and handle timeout
unsigned int read_ADC(void) {
    unsigned int timeout = 0;

    ADC12CTL0 |= ADC12ENC | ADC12SC;  // Enable and start conversion

    // Poll for the result or handle timeout
    while (!(ADC12IFGR0 & ADC12IFG0)) {
        timeout++;
        if (timeout > ADC_TIMEOUT) {
            // Handle timeout
            return 0xFFFF; // Return an error value (e.g., 0xFFFF)
        }
    }

    unsigned int result = ADC12MEM0; // Read the conversion result
    ADC12IFGR0 &= ~ADC12IFG0;        // Clear the interrupt flag
    ADC12CTL0 &= ~ADC12ENC;          // Disable ADC to clear any pending interrupts

    return result;
}

// Function to get the voltage of super capacitors via ADC
float voltage_adc_supercaps(void) {
    initialize_capready();             // Initialize the super cap voltage pin
    initialize_adc_supercaps();        // Initialize the ADC for super capacitors

    volatile unsigned int ADC_capture = read_ADC(); // Get ADC result
    float voltage = 0;                // Variable to store calculated voltage

    if (ADC_capture == 0xFFFF) {
        // Conversion failed
        voltage = 99;                 // Set error voltage
        return voltage;
    } else {
        voltage = convert_adc_to_voltage(ADC_capture); // Convert ADC result to voltage
        return voltage;
    }
}

// function to check the functionality of each supercapacitor
void check_supercap_functionality(void){
    switch_off_charge_cap_flag(0);
    switch_off_charge_cap_flag(1);
    switch_off_charge_cap_flag(2);
    switch_off_discharge_cap_flag();

    for(int i=0; i < 3; i++){
        switch_on_charge_cap_flag(i);
        //timer 2 min = 120 seconds = 120x4 = 0.25 seconds x 480
        startTimeoutTimer_TA3();
        while(timeoutCounterTA3 < 480){
            process_received_data();
        } // 480 times 0.25 seconds is 2 minutes
        stopTimeoutTimer_TA3();

        switch_on_discharge_cap_flag();

        float voltage_supercap_i = voltage_adc_supercaps();
        if(voltage_supercap_i > 1.7){ // above 1.7V
            supercap_functionality[i] = true;
        } else {
            supercap_functionality[i] = false;
        }
        if(i == 0) {
            if(supercap_functionality[i] == true){
                send_message(MSG_TYPE_DATA, PAYLOAD_SUPERCAP1_READY, sizeof(PAYLOAD_SUPERCAP1_READY) - 1);
            } else {
                send_message(MSG_TYPE_DATA, PAYLOAD_SUPERCAP1_NOT_READY, sizeof(PAYLOAD_SUPERCAP1_NOT_READY) - 1);
            }
        } else if(i == 1) {
            if(supercap_functionality[i] == true){
                send_message(MSG_TYPE_DATA, PAYLOAD_SUPERCAP2_READY, sizeof(PAYLOAD_SUPERCAP2_READY) - 1);
            } else {
                send_message(MSG_TYPE_DATA, PAYLOAD_SUPERCAP2_NOT_READY, sizeof(PAYLOAD_SUPERCAP2_NOT_READY) - 1);
            }
        } else if(i == 2) {
            if(supercap_functionality[i] == true){
                send_message(MSG_TYPE_DATA, PAYLOAD_SUPERCAP3_READY, sizeof(PAYLOAD_SUPERCAP3_READY) - 1);
            } else {
                send_message(MSG_TYPE_DATA, PAYLOAD_SUPERCAP3_NOT_READY, sizeof(PAYLOAD_SUPERCAP3_NOT_READY) - 1);
            }
        } else {}
    }
}

