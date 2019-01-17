/************************************** ****************************************/
/* Files to Include                                                           */
/******************************************************************************/

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

struct BusMessage busBuffer;
struct BusMessage busMessage;
unsigned char bitsReceived = 0;
unsigned char dataBlock = 0;

void setBusActive(uint8_t value) {
    BUS_SEND_ACTIVE_BIT = value;
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

void sendBusMessage() {
    INTCONbits.GIE = 0;
    //Activate bus busy signal and wait for a couple of ms
    setBusActive(1);

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

    uint8_t numberOfElements = sizeof (busMessage.rawData) / sizeof (busMessage.rawData[0]);
    for (uint8_t i = 1; i <= numberOfElements; i++) {
        uint8_t element = numberOfElements - i;
        uint8_t data = busMessage.rawData[element];

        for (uint8_t shift = 0; shift < 8; shift++) {
            lsb = data & 1;
            sendBit(lsb);

            //Shift the address bit stream to the left
            data = data >> 1;
        }
    }

    //Wait for a couple of ms to release the bus
    __delay_ms(6);

    //Release bus busy signal
    setBusActive(0);

    INTCONbits.GIE = 1;
}

void sendSerial(const char *text) {
    while (*text) {
        TXREG = *text;
        while (!TRMT);
        *text++;
    }
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
    
    sendSerial("Nikobus to Serial converter ready\r\n");
    sendSerial("May the force be with you!\r\n");

    while (true) {
        if(OERR) {
            CREN = 0;
            CREN = 1;
        }
        
        if (RCIF) {
            char u = RCREG;
            struct Address address;
            address.address[0] = 0x3E;
            address.address[1] = 0x65;
            address.address[2] = 0xAF;
            address.button = 1;
            
            busMessage = encodeAddress(&address);
            
            char text[12];
            sprintf(text, "%02X%02X%02X:%02X\r\n", address.address[0], address.address[1], address.address[2], address.button);

            sendSerial(text);
            sendBusMessage();
        }
    }
}

void resetBuffer() {
    busMessage = busBuffer;
    busBuffer.rawData[0] = 0;
    busBuffer.rawData[1] = 0;
    busBuffer.rawData[2] = 0;

    bitsReceived = 0;
    dataBlock = 2;
}

void interrupt isr(void) {
    unsigned char timer = TMR0;

    if (INTCONbits.T0IF) {
        // Clear interrupt flag
        // Disable timer interrupt. Will be enabled when a new input signal is detected
        INTCONbits.T0IF = 0;
        INTCONbits.T0IE = 0;
        TMR0 = TMR0_RELOAD;
        
        // Bus reader timeout. Check if a full message is read.
        resetBuffer();
    }

    if (INTCONbits.RABIF == 1) {
        if (BUS_RECEIVE_PULSE_BIT == 1) {
            //Reinitialize the timer and reload value.
            TMR0 = TMR0_RELOAD;
            if (bitsReceived == 9) {
                dataBlock = 1;
            } else if (bitsReceived == 17) {
                dataBlock = 0;
            }

            //ignore the first bit as it's a start bit.
            if (bitsReceived >= 1) {
                //Shift the raw dataBlock to the right.
                busBuffer.rawData[dataBlock] = busBuffer.rawData[dataBlock] >> 1;

                if ((timer > BUS_0_LOW_LIMIT) && (timer < BUS_0_HEIGH_LIMIT)) {
                    // Do nothing we already shifted a 0 in there
                } else if ((timer > BUS_1_LOW_LIMIT) && (timer < BUS_1_HEIGH_LIMIT)) {
                    // Swap msb to 1
                    busBuffer.rawData[dataBlock] = busBuffer.rawData[dataBlock] | 0b10000000;
                } else {
                    resetBuffer();
                }

                if (bitsReceived == 24) {
                    struct Address address = decodeMessage(&busBuffer);
                    
                    char text[12];
                    sprintf(text, "%02X%02X%02X:%02X\r\n", address.address[0], address.address[1], address.address[2], address.button);
                    sendSerial(text);
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
