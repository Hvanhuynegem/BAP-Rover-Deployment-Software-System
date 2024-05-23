#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

// Function to initialize P3.0 as an input pin
void initialize_umbilicalcord_pin_rover(void) {
    // Configure GPIO
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    P3DIR &= ~BIT0;                         // Set P3.0 as input (Umbilical_cord_status_bit)
}

// Function to read the status of P3.0 (umbilical cord rover)
bool read_umbilicalcord_pin_rover(void) {
    return (P3IN & BIT0) != 0;              // Return the status of P3.0
}

// Function to initialize Super cap voltage pin
void initialize_super_cap_voltage_pin(void) {

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    P3DIR &= ~BIT3;                         // Set P3.3 as input pin (Supercap voltage for all NEAS)
    P3SEL1 |= BIT3;                         // Configure P3.3 for ADC
    P3SEL0 |= BIT3;
}

// Function to initialize all 3 NEA's
void initialize_all_nea_pins(void) {

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

    // NEA 1
    P1DIR &= ~BIT2;                         // Set P1.2 as input pin (NEA Ready for NEA 1)
    P2DIR |= BIT4;                          // Set P2.4 as output pin (NEA Flag for NEA 1)
    P2OUT &= ~BIT4;                         // Initialize P2.4 to low (don't activate NEA 1)

    // NEA 2
    P3DIR &= ~BIT2;                         // Set P3.2 as input pin (NEA Ready for NEA 2)
    P2DIR |= BIT3;                          // Set P2.3 as output pin (NEA Flag for NEA 2)
    P2OUT &= ~BIT3;                         // Initialize P2.3 to low (don't activate NEA 2)

    // NEA 3
    P4DIR &= ~BIT7;                         // Set P4.7 as input pin (NEA Ready for NEA 3)
    P2DIR |= BIT7;                          // Set P2.7 as output pin (NEA Flag for NEA 3)
    P2OUT &= ~BIT7;                         // Initialize P2.7 to low (don't activate NEA 3)
}





// Function to read the status of the NEA ready bit
bool read_NEAready_status(volatile uint8_t *port_in, uint8_t pin) {
    return (*port_in & pin) != 0;  // Return the status of the specified pin
}


void RDS_electronics_status_check(void){
    //initialize the umbilicalcord pin and all the NEA pins
    initialize_umbilicalcord_pin_rover();
    initialize_all_neas();

    //Send a different massage to the lander depending on the status of the umbilical cord to the rover
    if(read_umbilicalcord_pin_rover()){
        send_message(uint8_t msg_type, const uint8_t *payload, uint8_t length) //TBD massage and data format
    }
    else{
        send_message(uint8_t msg_type, const uint8_t *payload, uint8_t length) //TBD massage and data format
    }


}
