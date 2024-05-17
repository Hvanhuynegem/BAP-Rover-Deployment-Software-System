
#include <msp430.h>
#define TXD BIT5  // Define TXD as P2.5
#define RXD BIT6  // Define RXD as P2.6

volatile unsigned char TXData = 1;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

  // Configure GPIO
  P1OUT &= ~BIT0;                           // Clear P1.0 output latch
  P1DIR |= BIT0;                            // Set P1.0 as output (LED)
  P2SEL1 |= TXD | RXD;                      // Configure P2.5 and P2.6 as UART pins
  P2SEL0 &= ~(TXD | RXD);
  PJSEL0 |= BIT4 | BIT5;                    // Configure XT1 pins

  // Output SMCLK on P2.2
  P2DIR |= BIT2;                            // Set P2.2 as output
  P2SEL1 |= BIT2;                           // Select the clock function for P2.2
  P2SEL0 &= ~BIT2;

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // XT1 Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
  CSCTL4 &= ~LFXTOFF;                       // Enable LFXT1
  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG);                // Test oscillator fault flag
  CSCTL0_H = 0;                             // Lock CS registers

  // Configure USCI_A0 for UART mode
  UCA0CTL1 |= UCSWRST;                     // Put eUSCI in reset
  UCA0CTL1 = UCSSEL__ACLK;                 // Select ACLK as the clock source
  UCA0BR0 = 3;                             // Set baud rate to 9600
  UCA0MCTLW |= 0x5300;                     // Modulation settings for 9600 baud
  UCA0BR1 = 0;
  UCA0CTL1 &= ~UCSWRST;                    // Initialize eUSCI (release from reset)
  UCA0IE |= UCTXIE;                        // Enable USCI_A0 TX interrupt

  __bis_SR_register(GIE);                  // Enable global interrupts

  while (1)
  {
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_A0_VECTOR __interrupt void USCI_A0_ISR(void) {
    switch(__even_in_range(UCA0IV,18)) {
        case 0x00: // Vector 0: No interrupts
            break;
        case 0x02: ... // Vector 2: UCRXIFG
            // The UCRXIFG interrupt flag is set each time a character is received and loaded into UCAxRXBUF.
            break;
        case 0x04: ... // Vector 4: UCTXIFG
            // The UCTXIFG interrupt flag is set by the transmitter to indicate that UCAxTXBUF is ready to accept
            // another character.
            break;
        case 0x06: ... // Vector 6: UCSTTIFG
            // START byte received interrupt. This flag is set when the UART module receives a START byte. This flag can
            // be cleared by writing 0 to it.
            break;
        case 0x08: ... // Vector 8: UCTXCPTIFG
            // Transmit complete interrupt. This flag is set after the complete UART byte in the internal shift register
            // including STOP bit is shifted out. This flag can be cleared by writing 0 to it.
            break;
        default: break;
    }
}
