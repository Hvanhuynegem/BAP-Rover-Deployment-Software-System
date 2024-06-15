#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include <actuatorControl.h>

void initializa_heat_resistor_pins(void){

    //MCU Heater off
    P1DIR |= BIT6;  // Set P1.6 as output pin (heater off)
    P1OUT &= ~BIT6; // Initially set P1.6 low

    //MCU Heater on
    P3DIR |= BIT6;  // Set P3.6 as output pin (heater on)
    P3OUT &= ~BIT6; // Initially set P3.6 low

    //Read heater active
    P3DIR &= ~BIT7;  // Set P1.4 as input pin (Read heater active)

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
}

void heat_resistor_control(void){



    if(temp_sens_1_def && temp_sens_2_def && temp_sens_3_def){
        // All temp sensors are defect leave it to analog
        P3OUT &= ~BIT4;
        P3OUT &= ~BIT4;     //Set both analog heater off on low and heater on on low
        // -> leave it to the analog system
    }
    else{
        case (geen kapot, 1 kapot en 2 kapot)
    }

}




