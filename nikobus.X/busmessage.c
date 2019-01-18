#include "busmessage.h"

void decodeBusMessage(uint24_t raw, struct BusMessage *busMessage) {
    busMessage->address = raw >> 2;
    busMessage->button = raw & 0b11;
}

uint24_t encodeBusMessage(struct BusMessage *busMessage) {
    uint24_t result = busMessage->address;
    result <<= 2;
    result = result | (unsigned long) busMessage->button;
    return result;
}

void resetBusMessage(struct BusMessage *busMessage) {
    busMessage->address = 0;
    busMessage->button = 0;
    busMessage->charPosition = 0;
}

bool isComplete(struct BusMessage *busMessage) {
    return busMessage->charPosition >= 9;
}

bool addChar(struct BusMessage *busMessage, char c) {
    if(busMessage->charPosition == 6) {
        if(c == ':') {
            busMessage->charPosition++;
            return true;
        } else {
            return false;
        }
    }

    if(!isxdigit(c)) {
        return false;
    }
    
    if(isComplete(busMessage)) {
        return false;
    }
            
    char hexValue = 0;
    if(isdigit(c)) {
        hexValue = c - 48;
    } else {
        hexValue = toupper(c) - 55;
    }
    
    if(busMessage->charPosition < 6) {
        busMessage->address = busMessage->address << 4;
        busMessage->address = busMessage->address | hexValue;
    } else {
        busMessage->button = busMessage->button << 4;
        busMessage->button = busMessage->button | hexValue;
    }
    busMessage->charPosition++;
    
    return true;
}