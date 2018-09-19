/*
 * File:   busmessage.h
 * Author: ron
 *
 * Created on 9 april 2013, 13:15
 */

#ifndef BUSMESSAGE_H
#define	BUSMESSAGE_H

    struct Address{
        unsigned char address[3];
        unsigned char button;
    };
    struct BusMessage {
        unsigned char rawData[3]; //Ready to send to the bus.
    };

    struct BusMessage encodeAddress(struct Address *address);
    struct Address decodeMessage(struct BusMessage *busMessage);


#endif	/* BUSMESSAGE_H */

