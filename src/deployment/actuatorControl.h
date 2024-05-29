/*
 * actuatorControl.h
 *
 *  Created on: 28 mei 2024
 *      Author: diederikaris
 */

#ifndef SRC_DEPLOYMENT_ACTUATORCONTROL_H_
#define SRC_DEPLOYMENT_ACTUATORCONTROL_H_


#ifndef RDS_ELECTRONICS_H
#define RDS_ELECTRONICS_H

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize P3.0 as an input pin for the umbilical cord status.
 */
void initialize_umbilicalcord_pin_rover(void);

/**
 * @brief Initialize P3.2 as an input pin and P1.3 as an output pin for the supercap status.
 */
void initialize_supercap(void);

/**
 * @brief Initialize all the pins for NEA (Nano Electrostatic Actuator) control.
 */
void initialize_all_nea_pins(void);

/**
 * @brief Initialize all the pins used to measure temperature via oscillation.
 */
void initialize_temperature_pins(void);

/**
 * @brief Read the status of P3.0 to determine the umbilical cord rover status.
 *
 * @return true if P3.0 is high, false otherwise.
 */
bool read_umbilicalcord_pin_rover(void);

/**
 * @brief Read the status of P3.2 to determine if the supercap is ready.
 *
 * @return true if P3.2 is high, false otherwise.
 */
bool read_supercap_ready(void);

/**
 * @brief Read the status of a specified pin to determine if the NEA is ready.
 *
 * @param port_in Pointer to the input port.
 * @param pin The pin to check the status of.
 * @return true if the specified pin is high, false otherwise.
 */
bool read_NEAready_status(volatile uint8_t *port_in, uint8_t pin);

/**
 * @brief Measure the frequency of a temperature pin.
 *
 * @param port_in Pointer to the input port.
 * @param pin The pin to measure the frequency of.
 * @return The frequency measured at the specified pin.
 */
float frequency_of_temperature_pin(volatile uint8_t *port_in, uint8_t pin);

/**
 * @brief Convert the frequency from a temperature sensor to a temperature value.
 *
 * @param frequency The frequency measured at the temperature sensor pin.
 * @return The temperature corresponding to the measured frequency.
 */
float transform_frequency_to_temperature(float frequency);

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

#endif // RDS_ELECTRONICS_H





#endif /* SRC_DEPLOYMENT_ACTUATORCONTROL_H_ */
