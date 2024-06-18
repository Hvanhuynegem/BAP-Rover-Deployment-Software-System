#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>


void RDS_electronics_status_check(void){


    //Send a different massage to the lander depending on the status of the umbilical cord to the rover
    bool status_umbilical_cord_rover = read_umbilicalcord_pin_rover();

    if(status_umbilical_cord_rover){
//        send_message(uint8_t msg_type, const uint8_t *payload, uint8_t length) //TBD massage and data format
    }
    else{
//        send_message(uint8_t msg_type, const uint8_t *payload, uint8_t length) //TBD massage and data format
    }

    //Is the supercap ready
    bool status_supercap_ready = read_supercap_ready();

    if(status_supercap_ready){
        P1OUT &= ~BIT3;          // At least turn off the supercap flag, as the supercap is not supposed to charge in this stage
        // Send message to lander that the supercap is charged
    }
    else{
        //Send message to lander that the supercap is not charged
    }

    //Check the status of the 4 NEA's
    bool status_NEA_1 = read_NEAready_status(&P1IN, BIT2);
    bool status_NEA_2 = read_NEAready_status(&P3IN, BIT3);
    bool status_NEA_3 = read_NEAready_status(&P4IN, BIT7);
    bool status_NEA_4 = read_NEAready_status(&P1IN, BIT7);

    // MOET NOG OMGESCHREVEN WORDEN -> DE NEAREADY IS NU EEN ACTIVE LOW

    if (status_NEA_1 && status_NEA_2 && status_NEA_3 && status_NEA_4){
        // Send message that non of the NEA's is activated already
    }
    else{
        if(status_NEA_1){
            //Send message that NEA 1 is not activated yet
        }
        else{
            //Send message that NEA 1 is already activated
        }
        if(status_NEA_2){
            //Send message that NEA 2 is not activated yet
        }
        else{
            //Send message that NEA 2 is already activated
        }
        if(status_NEA_3){
            //Send message that NEA 3 is not activated yet
        }
        else{
            //Send message that NEA 3 is already activated
        }
        if(status_NEA_4){
            //Send message that NEA 4 is not activated yet
        }
        else{
            //Send message that NEA 4 is already activated
        }
    }

    // Check temperatures with the temperature sensors
    readout_temperature_sensors();

}



