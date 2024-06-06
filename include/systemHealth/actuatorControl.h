/*
 * actuatorControl.h
 *
 *  Created on: 28 mei 2024
 *      Author: diederikaris
 */

#ifndef INCLUDE_SYSTEMHEALTH_ACTUATORCONTROL_H_
#define INCLUDE_SYSTEMHEALTH_ACTUATORCONTROL_H_

#ifndef ROVER_ELECTRONICS_H
#define ROVER_ELECTRONICS_H

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize P3.0 as an input pin for umbilical cord status.
 */
void initialize_umbilicalcord_pin_rover(void);

/**
 * @brief Initialize P3.2 as an input pin and P1.3 as an output pin for supercap readiness.
 */
void initialize_supercap(void);

/**
 * @brief Initialize all NEA pins.
 *
 * NEA 1:
 *  - P1.2 as input (NEA Ready)
 *  - P2.4 as output (NEA Flag)
 *
 * NEA 2:
 *  - P3.2 as input (NEA Ready)
 *  - P2.3 as output (NEA Flag)
 *
 * NEA 3:
 *  - P4.7 as input (NEA Ready)
 *  - P2.7 as output (NEA Flag)
 *
 * NEA 4:
 *  - P1.7 as input (NEA Ready)
 *  - P4.4 as output (NEA Flag)
 */
void initialize_all_nea_pins(void);

/**
 * @brief Initialize all temperature measurement pins.
 *
 * Temperature activation pin:
 *  - P3.4 as output
 *
 * Temperature sensors:
 *  - P3.6 as input (Temp sensor 1)
 *  - P3.5 as input (Temp sensor 2)
 *  - P2.2 as input (Temp sensor 3)
 */
void initialize_temperature_pins(void);

/**
 * @brief Read the status of P3.0 (umbilical cord rover).
 *
 * @return true if P3.0 is high, false otherwise.
 */
bool read_umbilicalcord_pin_rover(void);

/**
 * @brief Read the status of P3.2 (supercap readiness).
 *
 * @return true if P3.2 is high, false otherwise.
 */
bool read_supercap_ready(void);

/**
 * @brief Read the status of the specified NEA ready bit.
 *
 * @param port_in Pointer to the input port.
 * @param pin Pin number to read.
 * @return true if the specified pin is high, false otherwise.
 */
bool read_NEAready_status(volatile uint8_t *port_in, uint8_t pin);

/**
 * @brief Measure the frequency of the specified temperature pin.
 *
 * @param port_in Pointer to the input port.
 * @param pin Pin number to measure the frequency.
 * @return Frequency measured from the specified pin.
 */
float frequency_of_temperature_pin(volatile uint8_t *port_in, uint8_t pin);

/**
 * @brief Transform the measured frequency to temperature.
 *
 * @param frequency Frequency measured from the temperature sensor.
 * @return Corresponding temperature.
 */
float transform_frequency_to_temperature(float frequency);

/**
 * @brief Read the temperatures from all sensors and send messages if any sensor is defective.
 */
void readout_temperature_sensors(void);

/**
 * @brief Perform a status check of the RDS electronics.
 *
 * This function will:
 * - Check the status of the umbilical cord to the rover.
 * - Check if the supercap is ready.
 * - Check the status of the NEAs.
 * - Check temperatures using the temperature sensors.
 */

void RDS_electronics_status_check(void);

#endif // ROVER_ELECTRONICS_H



#endif /* INCLUDE_SYSTEMHEALTH_ACTUATORCONTROL_H_ */
