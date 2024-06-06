#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

////initialize the umbilicalcord pin, all the NEA pins and temperature pins
//initialize_umbilicalcord_pin_rover();
//initialize_supercap_ready();
//initialize_all_nea_pins();
//initialize_temperature_pins();


// Function to initialize P3.0 as an input pin
void initialize_umbilicalcord_pin_rover(void) {
    // Configure GPIO
    P3DIR &= ~BIT0;                         // Set P3.0 as input (Umbilical_cord_status_bit)
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to initialize P3.2 as an input pin
void initialize_supercap(void) {
    // Configure GPIO
    P3DIR &= ~BIT2;                         // Set P3.2 as input (supercap ready)
    P1DIR |= BIT3;                          // Set P1.3 as output pin (charge supercap flag)
    P1OUT &= ~BIT3;                         // Initialize P1.3 to low (don't charge the supercap).
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to initialize all 3 NEA's
void initialize_all_nea_pins(void) {


    // NEA 1
    P1DIR &= ~BIT2;                         // Set P1.2 as input pin (NEA Ready for NEA 1)
    P2DIR |= BIT7;                          // Set P2.4 as output pin (NEA Flag for NEA 1)
    P2OUT &= ~BIT7;                         // Initialize P2.4 to low (don't activate NEA 1)

    // NEA 2
    P3DIR &= ~BIT3;                         // Set P3.2 as input pin (NEA Ready for NEA 2)
    P2DIR |= BIT3;                          // Set P2.3 as output pin (NEA Flag for NEA 2)
    P2OUT &= ~BIT3;                         // Initialize P2.3 to low (don't activate NEA 2)

    // NEA 3
    P4DIR &= ~BIT7;                         // Set P4.7 as input pin (NEA Ready for NEA 3)
    P2DIR |= BIT4;                          // Set P2.7 as output pin (NEA Flag for NEA 3)
    P2OUT &= ~BIT4;                         // Initialize P2.7 to low (don't activate NEA 3)

    // NEA 4
    P1DIR &= ~BIT7;                         // Set P4.7 as input pin (NEA Ready for NEA 3)
    P4DIR |= BIT4;                          // Set P2.7 as output pin (NEA Flag for NEA 3)
    P4OUT &= ~BIT4;                         // Initialize P2.7 to low (don't activate NEA 3)

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to initialize all the pins used to measure the temperature via oscillation
void initialize_temperature_pins(void){

    // Pin to activate all the temperature sensors 3.4
    P3DIR |= BIT4;                          // Set P3.4 as output pin (Activate temp sens)
    P3OUT &= ~BIT4;                         // Initially set pin 3.4 low (don't activate temp sensors)

    // Temp sensor 1
    P3DIR &= ~BIT6;                         // Set P3.6 as input pin (Temp sensor 1)

    // Temp sensor 2
    P3DIR &= ~BIT5;                         // Set P3.5 as input pin (Temp sensor 2)

    // Temp sensor 3
    P2DIR &= ~BIT2;                         // Set P2.2 as input pin (Temp sensor 3)

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

// Function to read the status of P3.0 (umbilical cord rover)
bool read_umbilicalcord_pin_rover(void) {
    return (P3IN & BIT0) != 0;              // Return the status of P3.0
}

// Function to read the status of P3.2 (voltage of supercap above TBD threshold)
bool read_supercap_ready(void) {
    return (P3IN & BIT2) != 0;              // Return the status of P3.0
}

// Function to read the status of the NEA ready bit
bool read_NEAready_status(volatile uint8_t *port_in, uint8_t pin) {
    return (*port_in & pin) != 0;  // Return the status of the specified pin
}

// Function to measure
float frequency_of_temperature_pin(volatile uint8_t *port_in, uint8_t pin); //waarschijnlijk van dit type

float transform_frequency_to_temperature(float frequency);

//Function to read the temperature of every sensor or send an error if the sensor is defect
void readout_temperature_sensors(void){

    P3OUT |= BIT4;                         // Set pin 3.4 high (activate temp sensors)

    __delay_cycles(1000);                  // Ensure that the temperature sensors give a proper read out

    //Read out the three temperature sensors and sent message when defect.
    float frq_temp_sens_1 = frequency_of_temperature_pin(&P3IN, BIT6); // Determine the frequency of your selected temp_sens
    if(5000 <= frq_temp_sens_1 <= 15000){
        float temp_sens_1 = transform_frequency_to_temperature(frq_temp_sens_1);
        //Send message with the actual temperature
    }
       else{
           //Send message that Temp sens 1 is defect.
       }

    float frq_temp_sens_2 = frequency_of_temperature_pin(&P3IN, BIT5); // Determine the frequency of your selected temp_sens
    if(5000 <= frq_temp_sens_2 <= 15000){
        float temp_sens_2 = transform_frequency_to_temperature(frq_temp_sens_2);
        //Send message with the actual temperature
    }
    else{
        //Send message that Temp sens 1 is defect.
    }

    float frq_temp_sens_3 = frequency_of_temperature_pin(&P2IN, BIT2); // Determine the frequency of your selected temp_sens
    if(5000 <= frq_temp_sens_2 <= 15000){
        float temp_sens_3 = transform_frequency_to_temperature(frq_temp_sens_3);
        //Send message with the actual temperature
    }
    else{
        //Send message that Temp sens 1 is defect.
    }

       P3OUT &= ~BIT4;                      // Set pin 3.4 low again (turn of temp sensors)

}


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




