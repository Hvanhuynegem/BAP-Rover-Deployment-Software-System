/*
 * main_system_init.h
 *
 * This header file contains the function declarations and necessary includes for the main_system_init.cpp file, which provides functionalities for initializing basic functionalities of the MSP430FR5969.
 *
 * Author: Henri Vanhuynegem
 * created: 19/06/2024
 * Last edited: 19/06/2024
 *
 */

#ifndef MAIN_SYSTEM_INIT_H
#define MAIN_SYSTEM_INIT_H

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>


// External global variable to indicate if a timeout occurred
extern volatile bool timeoutOccurred;
extern volatile unsigned int timeoutCounterTA3;
/*
 * Initializes everything of the RDS.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void boot_up_initialisation(void);

/*
 * Initializes the general ADC settings.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_adc_general(void);

/*
 * Sets sub main clock (SMCLK) and digitally controlled oscillator (DCO) to 16 MHz.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void setup_SMCLK(void);

/*
 * Starts the timeout timer TA2.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void startTimeoutTimer_TA2(void);

/*
 * Stops the timeout timer TA2.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void stopTimeoutTimer_TA2(void);

/*
 * Starts the timeout timer TA3.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void startTimeoutTimer_TA3(void);

/*
 * Stops the timeout timer TA3.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void stopTimeoutTimer_TA3(void);

/*
 * Setup led light when mcu turns on.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void init_LED(void);

#endif // MAIN_SYSTEM_INIT_H
