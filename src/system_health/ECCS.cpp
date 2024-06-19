/*
 * ECCS.cpp file
 *
 * This file includes the functionalities for doing the Electronic Components Control System check.
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
#include "system_health_lib/ECCS.h"


void initialize_all_electronic_pins(void){
    // initialize the umbilicalcord readout pin 2.2
    initialize_umbilicalcord_pin_rover();
    // initialize the umbilicalcord detach pin 4.6
    initialize_umbilicalcord_detach_pin();
    // initialize the general adc settings
    initialize_adc_general();
    // initialize bus current readout pin 4.3
    initialize_bus_current_sense_pin();
    //initialize the adc settings for the bus current readout pin 4.3
    initialize_adc_bus_sense();
    // initialize bus flag pin 4.2 for conduction to the rover
    initialize_bus_flag_pin();
    // initialise temperature sensor pins
    initialize_temperature_pins();
    // initialize the timer B0 for temperature sensing
    setupTimer_B0();
    // initialize the charge and discharge cap flags
    initialize_charge_cap_flags();
    // initialize the supercap voltage pin 2.4
    initialize_capready();
    // initialize adc for supercaps
    initialize_adc_supercaps();
    // initialize all nea pins for ready signals and flags
    initialize_all_nea_pins();
    // initialize the led on pin 1.5
    init_LED();
    // initialize heater pins
    initialize_heat_resistor_pins();
}

void RDS_electronics_status_check(void){
    // check if umbilical cord is connected
    bool status_umbilical_cord_rover = umbilicalcord_rover_connected();

    // based on whether the umbilical cord is connected it sends either a correct connection or an error msg
    if(status_umbilical_cord_rover){
        send_message(MSG_TYPE_DATA, PAYLOAD_UMBILICAL_CONNECTED, sizeof(PAYLOAD_UMBILICAL_CONNECTED) - 1);
    }
    else{
        send_message(MSG_TYPE_ERROR, PAYLOAD_UMBILICAL_NOT_CONNECTED, sizeof(PAYLOAD_UMBILICAL_NOT_CONNECTED) - 1);
    }

    // bus current sensing, read the value of the bus and send it to the earth
    float bus_sense_voltage = voltage_adc_bus_sense();
    if(bus_sense_voltage == 99){
            send_message(MSG_TYPE_ERROR, PAYLOAD_BUS_SENSE_BROKEN, sizeof(PAYLOAD_BUS_SENSE_BROKEN) - 1);
    }
    else{
        // convert the float to the array of 5 characters and send message
        uint8_t PAYLOAD_BUS_SENSE_WORKS[] = "      is the current bus voltage"; // the first 5 characters are blank since they will be overriden
        float_to_uint8_array(bus_sense_voltage, PAYLOAD_BUS_SENSE_WORKS, 5);
        send_message(MSG_TYPE_DATA, PAYLOAD_BUS_SENSE_WORKS, sizeof(PAYLOAD_BUS_SENSE_WORKS) - 1);
    }

    // temperature sensors check
    // registers for temp sensor 1: TxxCCTLx = &TB0CCTL3, TxxCCRx = &TB0CCR3
    float temperature_of_sensor_1 = readout_temperature_sensor_1();

    // registers for temp sensor 2: TxxCCTLx = &TB0CCTL1, TxxCCRx = &TB0CCR1
    float temperature_of_sensor_2 = readout_temperature_sensor_2();

    // control the heat resistors and send a error message if a temp sensor is broken
    heat_resistor_control(temperature_of_sensor_1, temperature_of_sensor_2);

    // check the super capacitors.
    float supercap_voltage = voltage_adc_supercaps();

    if(supercap_voltage == 99) {
        // send an error message if the supercap voltage cannot be read
        send_message(MSG_TYPE_ERROR, PAYLOAD_SUPERCAP_VOLTAGE_ERROR, sizeof(PAYLOAD_SUPERCAP_VOLTAGE_ERROR) - 1);
    } else {
        if(supercap_voltage == 0){
            // send a message that the voltage is 0V
            send_message(MSG_TYPE_DATA, PAYLOAD_SUPERCAP_VOLTAGE_ZERO, sizeof(PAYLOAD_SUPERCAP_VOLTAGE_ZERO) - 1);
        } else {
            // convert the float to the array of 5 characters and send message
            uint8_t PAYLOAD_SUPERCAP_VOLTAGE_WORKS[] = "      is the current supercap voltage"; // the first 5 characters are blank since they will be overriden
            float_to_uint8_array(supercap_voltage, PAYLOAD_SUPERCAP_VOLTAGE_WORKS, 5);
            send_message(MSG_TYPE_DATA, PAYLOAD_SUPERCAP_VOLTAGE_WORKS, sizeof(PAYLOAD_SUPERCAP_VOLTAGE_WORKS) - 1);

            // set all the chargeCap flags and dischargecap flag to low
            initialize_charge_cap_flags();
        }
    }

    // NEA checkup
    //Check the status of the 4 NEA's
    bool status_NEA_1 = read_NEAready_status(&P3IN, BIT1);
    bool status_NEA_2 = read_NEAready_status(&P3IN, BIT2);
    bool status_NEA_3 = read_NEAready_status(&P3IN, BIT3);
    bool status_NEA_4 = read_NEAready_status(&P4IN, BIT7);

    // MOET NOG OMGESCHREVEN WORDEN -> DE NEAREADY IS NU EEN ACTIVE LOW

    if (status_NEA_1 && status_NEA_2 && status_NEA_3 && status_NEA_4){
        // Send message that non of the NEA's is activated already
        send_message(MSG_TYPE_DATA, PAYLOAD_ALL_NEA_READY, sizeof(PAYLOAD_ALL_NEA_READY) - 1);
    }
    else{
        if(status_NEA_1){
            //Send message that NEA 1 is not activated yet
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA1_READY, sizeof(PAYLOAD_NEA1_READY) - 1);
        }
        else{
            //Send message that NEA 1 is already activated
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA1_NOT_READY, sizeof(PAYLOAD_NEA1_NOT_READY) - 1);
        }
        if(status_NEA_2){
            //Send message that NEA 2 is not activated yet
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA2_READY, sizeof(PAYLOAD_NEA2_READY) - 1);
        }
        else{
            //Send message that NEA 2 is already activated
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA2_NOT_READY, sizeof(PAYLOAD_NEA2_NOT_READY) - 1);
        }
        if(status_NEA_3){
            //Send message that NEA 3 is not activated yet
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA3_READY, sizeof(PAYLOAD_NEA3_READY) - 1);
        }
        else{
            //Send message that NEA 3 is already activated
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA3_NOT_READY, sizeof(PAYLOAD_NEA3_NOT_READY) - 1);
        }
        if(status_NEA_4){
            //Send message that NEA 4 is not activated yet
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA4_READY, sizeof(PAYLOAD_NEA4_READY) - 1);
        }
        else{
            //Send message that NEA 4 is already activated
            send_message(MSG_TYPE_DATA, PAYLOAD_NEA4_NOT_READY, sizeof(PAYLOAD_NEA4_NOT_READY) - 1);
        }
    }

}






