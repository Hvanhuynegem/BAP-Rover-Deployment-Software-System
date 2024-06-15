//#include <msp430.h>
//#define TXD BIT5  // Define TXD as P2.5
//#define RXD BIT6  // Define RXD as P2.6
//
//volatile unsigned char TXData = 1;
//
//int main(void)
//{
//  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
//
//  // Configure GPIO
//  P2OUT &= ~BIT5;                           // Clear P1.0 output latch
//  P2DIR |= BIT5;                            // Set P1.0 as output (LED)
////  P2SEL1 |= TXD ;                      // Configure P2.5 and P2.6 as UART pins
////  P2SEL0 &= ~(TXD);
////  PJSEL0 |= BIT4 | BIT5;                    // Configure XT1 pins
//
//  // Disable the GPIO power-on default high-impedance mode to activate
//  // previously configured port settings
//  PM5CTL0 &= ~LOCKLPM5;
////
////  // XT1 Setup
////  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
////  CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
////  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
////  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
////  CSCTL4 &= ~LFXTOFF;                       // Enable LFXT1
////  do
////  {
////    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
////    SFRIFG1 &= ~OFIFG;
////  } while (SFRIFG1 & OFIFG);                // Test oscillator fault flag
////  CSCTL0_H = 0;                             // Lock CS registers
////
//  // Configure USCI_A0 for UART mode
////  UCA0CTL1 |= UCSWRST;
////  UCA0CTL1 = UCSSEL__ACLK;                  // Set ACLK = 32768 as UCBRCLK
////  UCA0BR0 = 3;                              // 9600 baud
////  UCA0MCTLW |= 0x5300;                      // 32768/9600 - INT(32768/9600)=0.41
////                                            // UCBRSx value = 0x53 (See UG)
////  UCA0BR1 = 0;
//  UCA0CTL1 &= ~UCSWRST;                     // Release from reset
//
//  while (1)
//  {
//    // Switch pin 2.5 on and off
//    P2OUT ^= BIT5;                          // Toggle LED on P1.0
//
////    while (!(UCA0IFG & UCTXIFG));           // Wait for TX buffer to be ready
////    UCA0TXBUF = TXData;                     // Load data onto buffer
////    TXData++;                               // Increment TX data
//
//    __delay_cycles(100000);                 // Add a delay to make the LED toggling visible
//  }
//}
