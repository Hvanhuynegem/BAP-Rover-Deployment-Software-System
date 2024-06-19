/*
 * heat_resistor_control.h
 *
 * This header file contains the function declarations and necessary includes for the heat_resistor_control.cpp file, which provides functionalities for controlling the heat resistors.
 *
 * Author: Henri Vanhuynegem
 * created: 19/06/2024
 * Last edited: 19/06/2024
 *
 */

#ifndef HEAT_RESISTOR_CONTROL_H
#define HEAT_RESISTOR_CONTROL_H

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "lander_communication_lib/lander_communication.h"
#include "lander_communication_lib/payload_messages.h"

/*
 * Initializes the pins for controlling the heat resistors.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_heat_resistor_pins(void);

/*
 * Sets P1.6 (MCU Heater off) low.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void MCU_heaterOff_low(void);

/*
 * Sets P1.6 (MCU Heater off) high.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void MCU_heaterOff_high(void);

/*
 * Sets P3.6 (MCU Heater on) low.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void MCU_heaterOn_low(void);

/*
 * Sets P3.6 (MCU Heater on) high.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void MCU_heaterOn_high(void);

/*
 * Checks if the heater is on (active low signal on P3.7).
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  bool : true if the heater is on, false otherwise
 */
bool is_heater_on(void);

/*
 * Controls the heat resistors based on two temperature readings.
 *
 * Parameters:
 *  float temperature1 : temperature from sensor 1
 *  float temperature2 : temperature from sensor 2
 *
 * Returns:
 *  void
 */
void heat_resistor_control(float temperature1, float temperature2);

/*
 * Controls the heat resistor based on one temperature reading.
 *
 * Parameters:
 *  float temperature : temperature from the sensor
 *
 * Returns:
 *  void
 */
void heat_resistor_control_one_sensor(float temperature);

/*
 * Controls the heat resistors based on two temperature readings.
 *
 * Parameters:
 *  float temperature1 : temperature from sensor 1
 *  float temperature2 : temperature from sensor 2
 *
 * Returns:
 *  void
 */
void heat_resistor_control_two_sensors(float temperature1, float temperature2);

#endif // HEAT_RESISTOR_CONTROL_H
