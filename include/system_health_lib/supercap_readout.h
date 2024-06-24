/*
 * supercap_readout.h
 *
 * This header file contains the function declarations and necessary includes for the supercap_readout.cpp file, which provides functionalities for initialising capacitor pins.
 *
 * Author: Diederik Aris
 * created: 28/05/2024
 * Last edited: 15/06/2024
 *
 */

#ifndef INCLUDE_SYSTEM_HEALTH_LIB_SUPERCAP_READOUT_H_
#define INCLUDE_SYSTEM_HEALTH_LIB_SUPERCAP_READOUT_H_

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>


#include "lander_communication_lib/lander_communication.h"
#include "lander_communication_lib/payload_messages.h"

extern bool supercap_functionality[3];

// Constants
#define ADC_MAX_VALUE        4095      // 12-bit ADC resolution (2^12 - 1)
#define MAX_VOLTAGE          3.64      // Reference voltage for ADC (measured 3.64 V)

// External global flag for ADC conversion failure
extern volatile bool adc_conversion_fail;


/*
 * Initializes the charge and discharge cap flags
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_charge_cap_flags(void);

void switch_on_charge_cap_flag(int flag);
void switch_off_charge_cap_flag(int flag);
void switch_on_discharge_cap_flag(void);
void switch_off_discharge_cap_flag(void);

/*
 * Initializes the Super cap voltage pin.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_capready(void);


/*
 * Initializes the ADC for super capacitors.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  void
 */
void initialize_adc_supercaps(void);

/*
 * Converts ADC value to voltage.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  Unsigned int : capture period
 */
unsigned int read_ADC(void);

/*
 * Converts ADC value to voltage.
 *
 * Parameters:
 *  volatile unsigned int adc_value : the ADC value to convert
 *
 * Returns:
 *  float : the calculated voltage
 */
float convert_adc_to_voltage(volatile unsigned int adc_value);

/*
 * Gets the voltage of super capacitors via ADC.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  float : the measured voltage
 */
float voltage_adc_supercaps(void);

/*
 * Check the supercapacitor functionality of each supercapacitor
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  None
 */
void check_supercap_functionality(void);

#endif /* INCLUDE_SYSTEM_HEALTH_LIB_SUPERCAP_READOUT_H_ */
