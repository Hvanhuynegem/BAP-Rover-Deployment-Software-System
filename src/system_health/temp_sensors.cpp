/*
 * temp_sensors.cpp file
 *
 * This file includes the functionalities for reading a frequency from an oscillator to determine the temperature in the environment where the oscillator is situated.
 *
 * Author: Diederik Aris
 * created: 28/05/2024
 * Last edited: 15/06/2024
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_health_lib/temp_sensors.h"

// Define the number of runs for the measurement
#define numberOfRuns 9

// Global variable to indicate if a timeout occurred
volatile bool timeoutOccurred = false;

// Timer A2 CCR0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A0_VECTOR))) Timer_A2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    // Handle CCR0 interrupt
    timeoutOccurred = true;  // Set timeout flag
    TA2CTL = MC_0;           // Stop the timer
    TA2CCTL0 &= ~CCIE;       // Disable interrupt
}

// Function to initialize all the pins used to measure the temperature via oscillation
void initialize_temperature_pins(void) {

    // Temp1out = pin to measure the temperature of the first sensor
    P3DIR &= ~BIT4;  // Set P3.4 as input pin (Temp sensor 1)
    P3SEL0 |= BIT4;  // Select Timer function for P3.4
    P3SEL1 &= ~BIT4; // Clear P3SEL1 for P3.4

    //Temp2out = pin to measure the temperature of the second sensor
    P1DIR &= ~BIT4;  // Set P1.4 as input pin (Temp sensor 2)
    P1SEL0 |= BIT4;  // Select Timer function for P1.4
    P1SEL1 &= ~BIT4; // Clear P3SEL1 for P1.4

    //TempOn1 = pin to put the first temperature sensor on
    P3DIR |= BIT5;  // Set P3.5 as output pin (control temp sensor 1)
    P3OUT &= ~BIT5; // Initially set P3.5 low

    //TempOn1 = pin to put the first temperature sensor on
    P1DIR |= BIT3;  // Set P1.3 as output pin (control temp sensor 1)
    P1OUT &= ~BIT3; // Initially set P1.3 low

    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
}

// Function to start the timeout timer
void startTimeoutTimer_TA2()
{
    timeoutOccurred = false;  // Reset timeout flag
    TA2CTL = TASSEL_2 + MC_1 + ID__4 + TACLR; // SMCLK = 16 MHz, up mode, input divider by 4, clear TAR
    TA2CCTL0 = CCIE;          // Enable interrupt
    TA2CCR0 = 60000;          // Set CCR0 value for the timer
}

// Function to stop the timeout timer
void stopTimeoutTimer_TA2()
{
    TA2CTL = MC_0;       // Stop the timer
    TA2CCTL0 = ~CCIE;    // Disable interrupt
    timeoutOccurred = false;  // Reset timeout flag
}

// Function to initialize the DCO (Digitally Controlled Oscillator)
void initializeDCO(void) {
    CSCTL0_H = CSKEY >> 8;      // Unlock CS registers
    CSCTL1 = DCOFSEL_4 | DCORSEL; // Set DCO to 16 MHz
    CSCTL2 = SELS__DCOCLK;      // Select DCO as the clock source for SMCLK
    CSCTL3 = DIVS__1;           // Set all dividers to 1
    CSCTL0_H = 0;               // Lock CS registers
}

// Function to convert frequency to temperature
float frequency_to_temperature(float frequency) {
    if (frequency >= 150 && frequency <= 800) {
        float capacitance = 0.0000022; // Capacitance in Farads
        float x = 1.0 / (0.4055 * 2.0 * capacitance * frequency); // Intermediate calculation using the frequency
        float temperature = (x - 1000.0) / 3.85; // Convert intermediate value to temperature
        return temperature; // Return the calculated temperature
    } else {
        return -99.0;  // returns an error value that is impossible to record
    }
}

// Function to set up Timer_B0 related to the temperature pins
void setupTimer(void) {
    TB0CTL = TBSSEL_2 | MC_2 | TBCLR | ID__4; // SMCLK, Continuous mode, clear TBR, divide the input clock by 4

    //For pin 3.4 (Temp1Out)
    TB0CCTL3 = CM_1 | CCIS_0 | SCS | CAP;     // Capture on rising edge, CCIxA, synchronize capture source, capture mode
    TB0CCTL3 &= ~CCIFG;                       // Clear interrupt flag

    //For pin 1.4 (Temp2Out)
    TB0CCTL1 = CM_1 | CCIS_0 | SCS | CAP;     // Capture on rising edge, CCIxA, synchronize capture source, capture mode
    TB0CCTL1 &= ~CCIFG;                       // Clear interrupt flag
}

// Function to calculate frequency from the period
float calculateFrequency(float period) {
    if (period == 0.0) {
        return 0;
    }
    const float clockFrequency = 4000000; // Assume SMCLK is 4 MHz
    return clockFrequency / period;       // Calculate the frequency
}

// Function to read out the temperature sensor measurements
void readout_temperature_sensor_n(volatile unsigned int* TxxCCTLx, volatile unsigned int* TxxCCRx, float* temperature) {
    volatile unsigned int firstCapture = 0; // First capture value
    volatile unsigned int secondCapture = 0; // Second capture value
    float singlePeriod = 0;                 // Single period measurement
    unsigned int singlePeriodMeasurements[numberOfRuns]; // Array to store period measurements
    int i, j; // Loop counters
    float frequency;

    // Loop to take numberOfRuns period measurements
    for (i = 0; i < numberOfRuns; i++) {
        startTimeoutTimer_TA2();  // Start the timeout timer

        // Wait for the first capture or timeout
        while (!(*TxxCCTLx & CCIFG) && !timeoutOccurred);

        firstCapture = *TxxCCRx;  // Read first capture value
        *TxxCCTLx &= ~CCIFG;      // Clear interrupt flag

        // Wait for the second capture or timeout
        while (!(*TxxCCTLx & CCIFG) && !timeoutOccurred);

        secondCapture = *TxxCCRx;  // Read second capture value
        *TxxCCTLx &= ~CCIFG;       // Clear interrupt flag

        if (timeoutOccurred) {
            stopTimeoutTimer_TA2();  // Stop the timer if timeout occurred
            singlePeriodMeasurements[i] = 1; // Set period to 1 if timeout occurred
        } else {
            stopTimeoutTimer_TA2();  // Stop the timer since measurement completed within time

            // Calculate the period
            if (secondCapture >= firstCapture) {
                singlePeriod = secondCapture - firstCapture;
            } else {
                singlePeriod = (secondCapture + 0x10000) - firstCapture;
            }

            singlePeriodMeasurements[i] = singlePeriod; // Store the period measurement
        }
    }

    // Sort the array to find the median
    unsigned int temp;
    for (i = 0; i < numberOfRuns; i++) {
        for (j = i + 1; j < numberOfRuns; j++) {
            if (singlePeriodMeasurements[i] > singlePeriodMeasurements[j]) {
                temp = singlePeriodMeasurements[i];
                singlePeriodMeasurements[i] = singlePeriodMeasurements[j];
                singlePeriodMeasurements[j] = temp;
            }
        }
    }

    // Median of numberOfRuns measurements
    float median = singlePeriodMeasurements[4];

    // Calculate the frequency using the median value
    frequency = calculateFrequency(median);

    // Check if the frequency is within the valid range
    if (frequency >= 150 && frequency <= 800) {
        *temperature = frequency_to_temperature(frequency); // Convert frequency to temperature
        // Send message with the actual temperature
    } else {
        // Send message that the temperature sensor is broken
    }
}


