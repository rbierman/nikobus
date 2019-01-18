/* 
 * File:   main.h
 * Author: ron
 *
 * Created on 21 april 2013, 18:03
 */

#ifndef MAIN_H
#define	MAIN_H

#include <stdint.h>        /* For uint8_t definition */
#include <stdio.h>
#include <stdbool.h>       /* For true/false definition */
#include <stdlib.h>        /* For strtol etc */
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */

#define BUS_SEND_ACTIVE_BIT PORTCbits.RC2
#define BUS_SEND_PULSE_BIT PORTBbits.RB4

#define BUS_RECEIVE_ACTIVE_BIT PORTAbits.RA4
#define BUS_RECEIVE_PULSE_BIT PORTAbits.RA2
#define BUS_RECEIVE_PULSE_INTERUPT_BIT IOCAbits.IOCA2
#endif	/* MAIN_H */

