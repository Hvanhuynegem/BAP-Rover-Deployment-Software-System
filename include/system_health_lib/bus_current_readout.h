/*
 * bus_current_readout.h
 *
 * This header file contains the function declarations and necessary includes for the bus_current_readout.cpp file, which provides functionalities for checking the bus current.
 *
 * Author: Henri Vanhuynegem
 * created: 18/06/2024
 * Last edited: 18/06/2024
 *
 */

#ifndef BUS_CURRENT_READOUT_H
#define BUS_CURRENT_READOUT_H

#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "system_health_lib/supercap_readout.h"

/*
 * Initializes pin 4.3 as an input pin for bus current sensing.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_bus_current_sense_pin(void);

/*
 * Initializes the ADC for bus current sensing.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_adc_bus_sense(void);

/*
 * Initializes pin 4.2 as an output pin for bus flag.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_bus_flag_pin(void);

/*
 * switches on pin 4.2 such that current flows to rover
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void switch_on_bus_flag_pin(void);

/*
 * switches off pin 4.2 such that current flows to rover
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void switch_off_bus_flag_pin(void);


/*
 * Reads the voltage from the bus current sense pin via ADC.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  float : the measured voltage
 */
float voltage_adc_bus_sense(void);


/*
 * Converts a float to a uint8_t array.
 *
 * Parameters:
 *  float value : the float value to convert
 *  uint8_t* array : pointer to the array where the converted values will be stored
 *  size_t n : length of the array (should be at least 4 to store the float)
 *
 * Returns:
 *  void
 */
void float_to_uint8_array(float value, uint8_t* array, size_t n);

/*
 * Converts a float to a uint8_t array with 7 digits.
 *
 * Parameters:
 *  float value : the float value to convert
 *  uint8_t* array : pointer to the array where the converted values will be stored
 *
 * Returns:
 *  void
 */
void float_to_uint8_array_2(float value, uint8_t* array);

#endif // BUS_CURRENT_READOUT_H
