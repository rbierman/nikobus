/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
#include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
#include <htc.h>        /* HiTech General Include File */
#endif

#include "lcd.h"

#define LCD_DATA PORTC
#define TRIS_DATA TRISC

#define LCD_E PORTCbits.RC3
#define TRIS_E TRISCbits.TRISC3

#define LCD_RW PORTBbits.RB6
#define TRIS_RW TRISBbits.TRISB6

#define LCD_RS PORTCbits.RC1
#define TRIS_RS TRISCbits.TRISC1

#define STROBE_DELAY __delay_us(500)

/* Initialize the lcd screen in 4 bit mode 2x16 autoincrement cursor
 */
void init_lcd()
{
        LCD_DATA &= 0x0f;
        TRIS_DATA &= 0x0F;

        TRIS_RW = 0;                    // All control signals made outputs
        TRIS_RS = 0;
        TRIS_E = 0;
        LCD_RW = 0;                     // R/W pin made low
        LCD_RS = 0;                     // Register select pin made low
        LCD_E = 0;                      // Clock pin made low

        // Delay for 15ms to allow for LCD Power on reset
        __delay_ms(15);
        //-------------------reset procedure through software----------------------
        writeCmdXLCD(0x30);
        __delay_ms(5);

        writeCmdXLCD(0x30);
        __delay_us(500);

        writeCmdXLCD(0x32);
	while( busyXLCD() ); //LCD in 4 bit mode after this.
//------------------------------------------------------------------------------------------


        // Set data interface width, # lines, font
        while(busyXLCD());                              // Wait if LCD busy
        writeCmdXLCD(0x28);                             // Function set cmd two lines 4 bits.

        // Turn the display on then off
        while(busyXLCD());                              // Wait if LCD busy
        writeCmdXLCD(DOFF&CURSOR_OFF&BLINK_OFF);        // Display OFF/Blink OFF
        while(busyXLCD());                              // Wait if LCD busy
        writeCmdXLCD(DON&CURSOR_ON&BLINK_ON);           // Display ON/Blink ON

        // Clear display
        clearLCD();

        // Set entry mode inc, no shift
        while(busyXLCD());              // Wait if LCD busy
        writeCmdXLCD(SHIFT_DISP_LEFT);   // Entry Mode

        // Set DD Ram address to 0
        while(busyXLCD());              // Wait if LCD busy
        setDDRamAddr(0x00);                // Set Display data ram address to 0

        return;
}


void writeCmdXLCD(uint8_t cmd)
{
        TRIS_DATA &= 0x0f; // Set all data ports to output.
        LCD_DATA &= 0x0f; // Clear data bit's
        LCD_DATA |= cmd&0xf0; // Set the first 4 bytes.

        LCD_RW = 0;                     // Set control signals for command
        LCD_E = 0;
        STROBE_DELAY;
        LCD_E = 1;                      // Clock command in
        STROBE_DELAY;
        LCD_E = 0;

        LCD_DATA &= 0x0f;               //Clear bits again
        LCD_DATA |= (cmd<<4)&0xf0;      //Shift lower 4 cmd bits in the data port.

        STROBE_DELAY;
        LCD_E= 1;                      // Clock command in
        STROBE_DELAY;
        LCD_E = 0;


        TRIS_DATA |= 0xf0; // Set dataport back to input.

}

uint8_t busyXLCD(void)
{
        LCD_RW = 1;                     // Set the control bits for read
        LCD_RS = 0;
        STROBE_DELAY;
        LCD_E = 1;                      // Clock in the command
        STROBE_DELAY;
        if(LCD_DATA&0x80)
        {
                LCD_E = 0;              // Reset clock line
                STROBE_DELAY;
                LCD_E = 1;              // Clock out other nibble
                STROBE_DELAY;
                LCD_E = 0;
                LCD_RW = 0;             // Reset control line
                return 1;               // Return TRUE
        }
        else                            // busy bit is low
        {
                LCD_E = 0;              // Reset clock line
                STROBE_DELAY;
                LCD_E = 1;              // Clock out other nibble
                STROBE_DELAY;
                LCD_E = 0;
                LCD_RW = 0;             // Reset control line
                return 0;               // Return FALSE
        }
}

/********************************************************************
*       Function Name:  putsXLCD
*       Return Value:   void
*       Parameters:     buffer: pointer to string
*       Description:    This routine writes a string of bytes to the
*                       Hitachi HD44780 LCD controller. The user
*                       must check to see if the LCD controller is
*                       busy before calling this routine. The data
*                       is written to the character generator RAM or
*                       the display data RAM depending on what the
*                       previous SetxxRamAddr routine was called.
********************************************************************/
void putsXLCD(char *data)
{
        while(*data)                  // write data to LCD up to null
        {
                while(busyXLCD());      // Wait while LCD is busy
                writeDataXLCD(*data); // write character to LCD
                data++;               // Increment buffer
        }
        return;
}

/********************************************************************
*       Function Name:  putrsXLCD
*       Return Value:   void
*       Parameters:     buffer: pointer to string
*       Description:    This routine writes a string of bytes to the
*                       Hitachi HD44780 LCD controller. The user
*                       must check to see if the LCD controller is
*                       busy before calling this routine. The data
*                       is written to the character generator RAM or
*                       the display data RAM depending on what the
*                       previous SetxxRamAddr routine was called.
********************************************************************/
void putrsXLCD(const char *data)
{
        while(*data)                  // Write data to LCD up to null
        {
                while(busyXLCD());      // Wait while LCD is busy
                writeDataXLCD(*data); // Write character to LCD
                data++;               // Increment buffer
        }
        return;
}

void putrsXLCDElement(const char *data, uint8_t element)
{
    uint8_t currentElement = 0;
    
    while (currentElement != element) {
        while (*data) {
            data++;
        }
        data++;
        currentElement++;
    }
    putrsXLCD(data);
}

void writeDataXLCD(char data)
{
        TRIS_DATA &= 0x0f;
        LCD_DATA &= 0x0f;
        LCD_DATA |= data&0xf0;

        LCD_RS = 1;                     // Set control bits
        LCD_RW = 0;
        STROBE_DELAY;
        LCD_E = 1;                      // Clock nibble into LCD
        STROBE_DELAY;
        LCD_E = 0;
        LCD_DATA &= 0x0f;
        LCD_DATA |= ((data<<4)&0xf0);

        STROBE_DELAY;
        LCD_E = 1;                      // Clock nibble into LCD
        STROBE_DELAY;
        LCD_E = 0;
                           
        TRIS_DATA |= 0xf0;
}


/********************************************************************
*       Function Name:  SetDDRamAddr                                *
*       Return Value:   void                                        *
*       Parameters:     DDaddr: display data address                *
*       Description:    This routine sets the display data address  *
*                       of the Hitachi HD44780 LCD controller. The  *
*                       user must check to see if the LCD controller*
*                       is busy before calling this routine.        *
********************************************************************/
void setDDRamAddr(unsigned char DDaddr)
{
    while(busyXLCD());      // Wait while LCD is busy
        TRIS_DATA &= 0x0f;                 // Make port output
        LCD_DATA &= 0x0f;                      // and write upper nibble
        LCD_DATA |= ((DDaddr | 0b10000000) & 0xf0);

        LCD_RW = 0;                             // Set control bits
        LCD_RS = 0;
        STROBE_DELAY;
        LCD_E = 1;                              // Clock the cmd and address in
        STROBE_DELAY;
        LCD_E = 0;

        LCD_DATA &= 0x0f;                      // Write lower nibble
        LCD_DATA |= ((DDaddr<<4)&0xf0);

        STROBE_DELAY;
        LCD_E = 1;                              // Clock the cmd and address in
        STROBE_DELAY;
        LCD_E = 0;
        TRIS_DATA |= 0xf0;                 // Make port input

        return;
}

void clearLCD()
{
    while(busyXLCD());                              // Wait if LCD busy
    writeCmdXLCD(0x01);
    while(busyXLCD());  
}