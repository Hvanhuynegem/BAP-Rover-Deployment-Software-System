/*
 * main_system_init.cpp file
 *
 * This file includes the functionalities for initialising basic functionalities of the msp430fr5969.
 *
 * Author: Henri Vanhuynegem
 * created: 19/06/2024
 * Last edited: 19/06/2024
 *
 */

// import packages
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

// include header files
#include "system_health_lib/main_system_init.h"
#include "system_health_lib/ECCS.h"

// Global variable to indicate if a timeout occurred
volatile bool timeoutOccurred = false;
volatile unsigned int timeoutCounterTA3 = 0;

void boot_up_initialisation(void){
    setup_SMCLK();
    uart_configure();
    initialize_all_electronic_pins();

}

// Function to initialize the general ADC settings
void initialize_adc_general(void) {
    ADC12CTL0 &= ~ADC12ENC;            // Disable ADC12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON; // Set sampling time, S&H=16, ADC12 on
    ADC12CTL1 = ADC12SHP;              // Use sampling timer
    ADC12CTL2 |= ADC12RES_2;           // 12-bit conversion results
    ADC12IER2 |= ADC12TOVIE | ADC12OVIE; // Enable overflow interrupts
    ADC12CTL0 |= ADC12ENC;             // Enable conversions
    PM5CTL0 &= ~LOCKLPM5;              // Disable the GPIO power-on default high-impedance mode
}

// function to set sub main clock and digital clock oscillator to 16MHz
void setup_SMCLK(void)
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK operation beyond 8MHz
    FRCTL0 = FRCTLPW | NWAITS_1;

    // Set DCO frequency to 16 MHz
    CSCTL0_H = CSKEY >> 8;                      // Unlock CS registers
    CSCTL1 = DCOFSEL_4;                         // Set DCO to 16 MHz (DCORSEL selects high range, DCOFSEL_4 sets it to 16 MHz)
    CSCTL1 |= DCORSEL;
    CSCTL2 = SELS__DCOCLK | SELM__DCOCLK;       // Set SMCLK and DCO source to 16MHz
    CSCTL3 = DIVS__1 | DIVM__1;                 // SMCLK = DCO = 16MHz
    CSCTL0_H = 0;                               // Lock CS registers
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

// Function to start the timeout timer with the lowest clock frequency
void startTimeoutTimer_TA3() {
    timeoutCounterTA3 = 0;  // Reset timeout counter
    TA3CTL = TASSEL_2 + MC_1 + ID__8 + TACLR; // SMCLK = 16 MHz, up mode, input divider by 8, clear TAR
    TA3EX0 = TAIDEX_7;  // Expanded divider by 8
    TA3CCTL0 = CCIE;    // Enable interrupt
    TA3CCR0 = 60000;    // Set CCR0 value for the timer
}

// Function to stop the timeout timer
void stopTimeoutTimer_TA3() {
    TA3CTL = MC_0;       // Stop the timer
    TA3CCTL0 = 0;        // Disable interrupt (clearing the CCIE bit)
    timeoutCounterTA3 = 0;  // Reset timeout counter
}



// Initialize LED
void init_LED(void) {
    P1DIR |= BIT5;   // Set P1.5 as output
    P1OUT |= BIT5;  // Set P1.5 high (active high, so reset state is low) this shows that the mcu is on
}



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


// Timer A3 CCR0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER3_A0_VECTOR
__interrupt void Timer_A3_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER3_A0_VECTOR))) Timer_A3_ISR(void)
#else
#error Compiler not supported!
#endif
{
    // Handle CCR0 interrupt
    TA3CCTL0 &= ~CCIFG;  // Clear interrupt flag
    timeoutCounterTA3++;
}
