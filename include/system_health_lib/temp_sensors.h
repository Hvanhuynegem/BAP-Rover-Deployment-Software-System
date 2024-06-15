/*
 * temp_sensors.h
 *
 * This header file contains the function declarations and necessary includes for the temp_sensors.cpp file, which provides functionalities for reading a frequency from an oscillator to determine the temperature in the environment where the oscillator is situated.
 *
 * Author: Diederik Aris
 * created: 28/05/2024
 * Last edited: 15/06/2024
 *
 */

#ifndef INCLUDE_SYSTEM_HEALTH_LIB_TEMP_SENSORS_H_
#define INCLUDE_SYSTEM_HEALTH_LIB_TEMP_SENSORS_H_

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

// Define the number of runs for the measurement
#define numberOfRuns 9

// External global variable to indicate if a timeout occurred
extern volatile bool timeoutOccurred;


/*
 * Initializes all the pins used to measure the temperature via oscillation.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_temperature_pins(void);

/*
 * Starts the timeout timer.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void startTimeoutTimer_TA2(void);

/*
 * Stops the timeout timer.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void stopTimeoutTimer_TA2(void);

/*
 * Initializes the DCO (Digitally Controlled Oscillator).
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initializeDCO(void);

/*
 * Converts frequency to temperature.
 *
 * Parameters:
 *  float frequency : the frequency to convert to temperature
 *
 * Returns:
 *  float : the calculated temperature
 */
float frequency_to_temperature(float frequency);

/*
 * Sets up Timer_B0 related to the temperature pins.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void setupTimer(void);

/*
 * Calculates frequency from the period.
 *
 * Parameters:
 *  float period : the period from which to calculate the frequency
 *
 * Returns:
 *  float : the calculated frequency
 */
float calculateFrequency(float period);

/*
 * Reads out the temperature sensor measurements.
 *
 * Parameters:
 *  volatile unsigned int* TxxCCTLx : pointer to Timer B control register
 *  volatile unsigned int* TxxCCRx : pointer to Timer B capture/compare register
 *  float* temperature : pointer to store the calculated temperature
 *
 * Returns:
 *  void
 */
void readout_temperature_sensor_n(volatile unsigned int* TxxCCTLx, volatile unsigned int* TxxCCRx, float* temperature);


#endif /* INCLUDE_SYSTEM_HEALTH_LIB_TEMP_SENSORS_H_ */
