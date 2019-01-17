/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
#include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
#include <htc.h>        /* HiTech General Include File */
#endif

#include "main.h"


/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */
void InitApp(void) {

    ANSEL = 0; // Disable analog input because used as GIO
    ANSELH = 0; //Disable analog input because used as GIO
    CM1CON0 = 0; //Disable comparator because used as GIO
    CM2CON0 = 0; //Disable comparator because used as GIO

    TRISA = 0b00010100; // PORT A RA2, RA4 input
    TRISB = 0; // PORT B All outputs because not used.
    TRISC = 0b00000011; // PORT C All outputs Except for RC0 and RC1 as there was a PCB bug in the first version. In that version these ports are connected to RA2 and RA4*/

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;

    //Disable weak-pull up
    OPTION_REGbits.nRABPU = 1;

    /* Setup analog functionality and port direction */

    /* Initialize peripherals */
    // Timer0 Registers:
    // Prescaler=1:32; TMR0 Preset=87; Freq=369,82249Hz; Period=2,704 ms
    OPTION_REGbits.T0CS = 0;// bit 5 TMR0 Clock Source Select bit:0=Internal Clock (CLKO) / 1=Transition on T0CKI pin
    OPTION_REGbits.T0SE = 0;// bit 4 TMR0 Source Edge Select bit: 0=low/high / 1=high/low
    OPTION_REGbits.PSA  = 0;// bit 3 Prescaler Assignment bit: 0=Prescaler is assigned to the Timer0
    OPTION_REGbits.PS2  = 1;// bits 2-0  PS2:PS0: Prescaler Rate Select bits
    OPTION_REGbits.PS1  = 0;
    OPTION_REGbits.PS0  = 0;

    // Timer1 Registers:
    // Prescaler=1:2; TMR1 Preset=15536; Freq=20,00Hz; Period=0,05 s
//    T1CONbits.T1CKPS1 = 0; // bits 5-4  Prescaler Rate Select bits
//    T1CONbits.T1CKPS0 = 1;
//    T1CONbits.T1OSCEN = 0; // bit 3 Timer1 Oscillator Enable Control: bit 1=on
//    T1CONbits.nT1SYNC = 1; // bit 2 Timer1 External Clock Input Synchronization Control bit: 1=Do not synchronize external clock input
//    T1CONbits.TMR1CS = 0; // bit 1 Timer1 Clock Source Select bit: 0=Internal clock (FOSC/4) / 1 = External clock from pin T1CKI (on the rising edge)
//    T1CONbits.TMR1ON = 1; // bit 0 enables timer
//    TMR1H = 0x3C;     // preset for timer1 MSB register
//    TMR1L = 0xB0;     // preset for timer1 LSB register

    // Timer2 Registers:
    // Prescaler=1:4; TMR2 PostScaler=1:4; PR2=63 - Freq = 1.984,12698Hz - Period = 0,504 ms
    //T2CON |       = 24;// bits 6-3 Post scaler 1:1 thru 1:16
    //T2CON.TOUTPS3 = 0;
    //T2CON.TOUTPS2 = 0;
    //T2CON.TOUTPS1 = 1;
    //T2CON.TOUTPS0 = 1;
    //T2CON.TMR2ON  = 1;// Timer2 on bit: 1=Timer2 is on;
    //T2CON.T2CKPS1 = 0;// bits 1-0  Prescaler Rate Select bits
    //T2CON.T2CKPS0 = 1;
//    T2CON = 0b00011101;
//    PR2 = 63;           // PR2 (Timer2 Match value)


    //Setup serial communication
    SPBRGH = 0;
    SPBRG = 12;
    BRGH = 0;
    BRG16 = 0;
    SYNC = 0;
    SPEN = 1;

    //Enable transmission
    TXEN = 1;
    CREN = 1;
    
    // Global Interrupt Enable bit
    INTCONbits.GIE = 1;
    // Peripheral Interrupt Enable bit
    INTCONbits.PEIE = 1;
    
    // PORTA/PORTB Change Interrupt Enable bit
    INTCONbits.RABIE = 1;
    
    //RABIF: PORTA/PORTB Change Interrupt Flag bit
    // 1 = When at least one of the PORTA or PORTB general purpose I/O pins changed state (must be cleared in software)
    // 0 = None of the PORTA or PORTB general purpose I/O pins have changed state
    INTCONbits.RABIF = 0;

    BUS_RECEIVE_PULSE_INTERUPT_BIT = 1; //Enable interrupt detection for RA2
    PIE1bits.TMR1IE = 0; // Enable interrupt for timer1;
    PIE1bits.T2IE = 0; //Enable interrupt for timer2;
    PIE1bits.ADIE = 0; //A/D Converter (ADC) Interrupt Enable bit
}

