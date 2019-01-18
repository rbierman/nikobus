/************************************** ****************************************/
/* Files to Include                                                           */
/******************************************************************************/
// 
//    busMessage.address = 0x3E65AF;
//    busMessage.button = 1;


#if defined(__XC)
#include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
#include <htc.h>        /* HiTech General Include File */
#endif

#include "main.h"
#include "user.h"          /* User funct/params, such as InitApp */
#include "busmessage.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */

/*****************************************************************************/
#define TMR0_RELOAD  87 // preset for timer0 rgister

#define BUS_0_LOW_LIMIT 149 // 1,0ms
#define BUS_0_HEIGH_LIMIT 193 // 1,7ms
#define BUS_1_LOW_LIMIT 118 // 0,5ms
#define BUS_1_HEIGH_LIMIT 149 // 1,0 ms

uint24_t busBuffer;
struct BusMessage busMessage;
struct BusMessage receivedMessage;
char message[12];

unsigned char bitsReceived = 0;
unsigned char dataBlock = 0;

void setBusActive() {
    BUS_SEND_ACTIVE_BIT = 1;
    __delay_ms(10);
}

void setBusInActive() {
    BUS_SEND_ACTIVE_BIT = 0;
    __delay_ms(10);
}

void sendPulse() {
    BUS_SEND_PULSE_BIT = 1;
    __delay_us(150);
    BUS_SEND_PULSE_BIT = 0;
}

void sendBit(uint8_t value) {
    if (value == 1) {
        __delay_us(600);
    } else {
        __delay_us(1250);
    }

    sendPulse();
}

void sendSerialChar(char c) {
    TXREG = c;
    while (!TRMT);
}

void sendSerialString(const char *text) {
    while (*text) {
        TXREG = *text;
        while (!TRMT);
        *text++;
    }
}

void sendBusMessage() {
    INTCONbits.GIE = 0;
    //Activate bus busy signal and wait for a couple of ms
    setBusActive();

    //Send three initial bits
    sendBit(1);
    sendBit(1);
    sendBit(1);

    //Wait for 3,0 ms
    __delay_us(2800);

    //Send start pulse
    sendPulse();

    uint8_t lsb = 0;
    //Send the encoded data buffer to the bus (from back to front)

    uint24_t raw = encodeBusMessage(&busMessage);

    for (int shift = 0; shift < sizeof(raw) * CHAR_BIT; shift++) {        
        lsb = raw & 1;
        sendBit(lsb);

        //Shift the address bit stream to the right
        raw >>= 1;
    }

    //Wait for a couple of ms to release the bus
    __delay_ms(6);

    //Release bus busy signal
    setBusInActive();

    INTCONbits.GIE = 1;
}

/******************************************************************************/
/* RA3: Input                                                                 */
/* RC2: Lower bus to 8,0V (Bus busy)                                          */
/* RB4: Lower bus to 4,5V (Pulse)                                             */

/******************************************************************************/
void main(void) {
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    
    sendSerialString("Nikobus to Serial converter ready\r\n");
    sendSerialString("May the force be with you!\r\n");    
    
    resetBusMessage(&busMessage);
    
    while (true) {
        if(OERR) {
            CREN = 0;
            CREN = 1;
        }
        
        if (RCIF) {
            char c = RCREG;
            
            if(c == 13 ) {
                if(isComplete(&busMessage)) {
                    strcpy(message, "\r\nOK\r\n");
                    sendBusMessage();
                } else {
                    strcpy(message, "\r\nInvalid\r\n");
                }
                resetBusMessage(&busMessage);
                sendSerialString(message);               
            }

            if(addChar(&busMessage, c)) {
                sendSerialChar(c);
            }
        }
    }
}
    
void interrupt isr(void) {
    unsigned char timer = TMR0;

    if (INTCONbits.T0IF) {
        // Clear interrupt flag
        // Disable timer interrupt. Will be enabled when a new input signal is detected
        INTCONbits.T0IF = 0;
        INTCONbits.T0IE = 0;
        TMR0 = TMR0_RELOAD;
        
        // Bus reader timeout. Reset Buffer
        busBuffer = 0;
        bitsReceived = 0;
    }

    if (INTCONbits.RABIF == 1) {
        if (BUS_RECEIVE_PULSE_BIT == 1) {
            //Reinitialize the timer and reload value.
            TMR0 = TMR0_RELOAD;
    
            //ignore the first bit as it's a start bit.
            if (bitsReceived >= 1) {
                //Shift the raw dataBlock to the right.
                busBuffer >>= 1;

                if ((timer > BUS_0_LOW_LIMIT) && (timer < BUS_0_HEIGH_LIMIT)) {
                    // Do nothing we already shifted a 0 in there
                } else if ((timer > BUS_1_LOW_LIMIT) && (timer < BUS_1_HEIGH_LIMIT)) {
                    // Swap msb to 1
                    busBuffer = busBuffer | (unsigned long)0x800000;
                } else {
                    busBuffer = 0;
                    bitsReceived = 0;
                }

                if (bitsReceived == 24) {
                    decodeBusMessage(busBuffer, &receivedMessage);

                    sprintf(message, "%06lX:%02X\r\n", (unsigned long)receivedMessage.address, receivedMessage.button);
                    sendSerialString(message);
                }
            }

            bitsReceived++;
        }

        //RABIF: PORTA/PORTB Change Interrupt Flag bit
        INTCONbits.RABIF = 0;
        
        // T0IE: Timer0 Overflow Interrupt Enable bit
        INTCONbits.T0IE = 1;
    }
}
