/*
 * heat_resistor_control.cpp file
 *
 * This file includes the functionalities for controlling the heat resistors.
 *
 * Author: Henri Vanhuynegem
 * created: 19/06/2024
 * Last edited: 19/06/2024
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "system_health_lib/heat_resistor_control.h"

void initialize_heat_resistor_pins(void){

    //MCU Heater off
    P1DIR |= BIT6;  // Set P1.6 as output pin (heater off)
    P1OUT &= ~BIT6; // Initially set P1.6 low

    //MCU Heater on
    P3DIR |= BIT6;  // Set P3.6 as output pin (heater on)
    P3OUT &= ~BIT6; // Initially set P3.6 low

    //Read heater active
    P3DIR &= ~BIT7;  // Set P3.7 as input pin (Read heater active)

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

void MCU_heaterOff_low(void){
    P1OUT &= ~BIT6; // Initially set P1.6 low
}

void MCU_heaterOff_high(void){
    P1OUT |= BIT6; // Initially set P1.6 low
}

void MCU_heaterOn_low(void){
    P3OUT &= ~BIT6; // Initially set P3.6 low
}

void MCU_heaterOn_high(void){
    P3OUT |= BIT6; // Initially set P3.6 low
}

bool is_heater_on(void){
    return (P3IN & BIT7) == 0; // it is an active low signal
}

void heat_resistor_control(float temperature1, float temperature2) {
    int condition = 0;
    if (temperature1 == -99 && temperature2 == -99) {
        condition = 1;
    } else if (temperature1 == -99) {
        condition = 2;
    } else if (temperature2 == -99) {
        condition = 3;
    } else {
        condition = 4;
    }

    switch (condition) {
        case 1:
            // set the heaterOff and heaterOn both low
            MCU_heaterOff_low();
            MCU_heaterOn_low();
            // send an error message for sensor 1
            send_message(MSG_TYPE_ERROR, PAYLOAD_TEMP_SENSOR_1_BROKEN, sizeof(PAYLOAD_TEMP_SENSOR_1_BROKEN) - 1);
            // send an error message for sensor 2
            send_message(MSG_TYPE_ERROR, PAYLOAD_TEMP_SENSOR_2_BROKEN, sizeof(PAYLOAD_TEMP_SENSOR_2_BROKEN) - 1);
            break;

        case 2:
            // send an error message for sensor 1
            send_message(MSG_TYPE_ERROR, PAYLOAD_TEMP_SENSOR_1_BROKEN, sizeof(PAYLOAD_TEMP_SENSOR_1_BROKEN) - 1);
            heat_resistor_control_one_sensor(temperature2);
            // convert the float to the array of 5 characters and send message
            uint8_t PAYLOAD_TEMP_SENSOR_2_WORKS[] = "        is the current temperature of sensor 2"; // the first 7 characters are blank since they will be overriden
            float_to_uint8_array_2(temperature2, PAYLOAD_TEMP_SENSOR_2_WORKS);
            send_message(MSG_TYPE_DATA, PAYLOAD_TEMP_SENSOR_2_WORKS, sizeof(PAYLOAD_TEMP_SENSOR_2_WORKS) - 1);
            break;

        case 3:
            // send an error message for sensor 2
            send_message(MSG_TYPE_ERROR, PAYLOAD_TEMP_SENSOR_2_BROKEN, sizeof(PAYLOAD_TEMP_SENSOR_2_BROKEN) - 1);
            heat_resistor_control_one_sensor(temperature1);
            uint8_t PAYLOAD_TEMP_SENSOR_1_WORKS[] = "        is the current temperature of sensor 1"; // the first 7 characters are blank since they will be overriden
            float_to_uint8_array_2(temperature1, PAYLOAD_TEMP_SENSOR_1_WORKS);
            send_message(MSG_TYPE_DATA, PAYLOAD_TEMP_SENSOR_1_WORKS, sizeof(PAYLOAD_TEMP_SENSOR_1_WORKS) - 1);
            break;

        case 4:
            heat_resistor_control_two_sensors(temperature1, temperature2);
            // convert the float to the array of 5 characters and send message
            uint8_t PAYLOAD_TEMP_SENSOR_1_WORKS_2[] = "        is the current temperature of sensor 1"; // the first 7 characters are blank since they will be overriden
            float_to_uint8_array_2(temperature1, PAYLOAD_TEMP_SENSOR_1_WORKS_2);
            send_message(MSG_TYPE_DATA, PAYLOAD_TEMP_SENSOR_1_WORKS_2, sizeof(PAYLOAD_TEMP_SENSOR_1_WORKS_2) - 1);
            // convert the float to the array of 5 characters and send message
            uint8_t PAYLOAD_TEMP_SENSOR_2_WORKS_2[] = "        is the current temperature of sensor 2"; // the first 7 characters are blank since they will be overriden
            float_to_uint8_array_2(temperature2, PAYLOAD_TEMP_SENSOR_2_WORKS_2);
            send_message(MSG_TYPE_DATA, PAYLOAD_TEMP_SENSOR_2_WORKS_2, sizeof(PAYLOAD_TEMP_SENSOR_2_WORKS_2) - 1);
            break;

        default:
            // This case should never be reached
            break;
    }
}


void heat_resistor_control_one_sensor(float temperature){
    // check if the heater is on
    bool heater_status = is_heater_on();

    if(heater_status){
        // if heater on and above 40 degrees celcius, turn off
        if(temperature > 40){
            MCU_heaterOn_low();
            MCU_heaterOff_high();
        }
    } else {
        // if heater off and below 20 degrees celcius, turn on
        if(temperature < 20){
            MCU_heaterOff_low();
            MCU_heaterOn_high();
        }
    }
}

void heat_resistor_control_two_sensors(float temperature1, float temperature2){
    // check if the heater is on
    bool heater_status = is_heater_on();

    if(heater_status){
        // if heater on and above 40 degrees celcius, turn off
        if(temperature1 > 40 || temperature2 > 40){
            MCU_heaterOn_low();
            MCU_heaterOff_high();
        }
    } else {
        // if heater off and below 20 degrees celcius, turn on
        if(temperature1 < 20 || temperature2 < 20){
            MCU_heaterOff_low();
            MCU_heaterOn_high();
        }
    }
}




