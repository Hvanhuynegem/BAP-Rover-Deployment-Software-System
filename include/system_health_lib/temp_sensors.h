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

#include "system_health_lib/main_system_init.h"

// Define the number of runs for the measurement
#define numberOfRuns 9

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
void setupTimer_B0(void);

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
 *
 * Returns:
 *  float temperature: the measured temperature
 */
float readout_temperature_sensor_n(volatile unsigned int* TxxCCTLx, volatile unsigned int* TxxCCRx);

/*
 * Reads out the temperature sensor measurements from sensor 1.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  float temperature: the measured temperature
 */
float readout_temperature_sensor_1(void);

/*
 * Reads out the temperature sensor measurements from sensor 2.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  float temperature: the measured temperature
 */
float readout_temperature_sensor_2(void);

#endif /* INCLUDE_SYSTEM_HEALTH_LIB_TEMP_SENSORS_H_ */
