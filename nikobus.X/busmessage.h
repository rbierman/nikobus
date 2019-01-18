/*
 * File:   busmessage.h
 * Author: ron
 *
 * Created on 9 april 2013, 13:15
 */

#ifndef BUSMESSAGE_H
#define	BUSMESSAGE_H

    #include <stdint.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <ctype.h>

    struct BusMessage {
        uint24_t address;
        uint8_t button;
        uint8_t charPosition; 
    };
    
    void decodeBusMessage(uint24_t raw, struct BusMessage *busMessage);
    uint24_t encodeBusMessage(struct BusMessage *busMessage);
    
    bool isComplete(struct BusMessage *busMessage);
    bool addChar(struct BusMessage *busMessage, char c);
    void resetBusMessage(struct BusMessage *busMessage);
#endif	/* BUSMESSAGE_H */

