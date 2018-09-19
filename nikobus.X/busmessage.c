#include "busmessage.h"

struct Address decodeMessage(struct  BusMessage *busMessage) {
    struct Address address;
    address.address[0] = busMessage->rawData[0] >> 2;
    address.address[1] = ((busMessage->rawData[0] & 0b00000011) << 6) | (busMessage->rawData[1] >> 2);
    address.address[2] = ((busMessage->rawData[1] & 0b00000011) << 6) | (busMessage->rawData[2] >> 2);

    address.button = busMessage->rawData[2] & 0b00000011;
    return address;
}

struct BusMessage encodeAddress(struct Address *address) {
    struct BusMessage busMessage;
    busMessage.rawData[0] = (address->address[0] << 2) | (address->address[1] >> 6);
    busMessage.rawData[1] = (address->address[1] << 2) | (address->address[2] >> 6);
    busMessage.rawData[2] = (address->address[2] << 2) | (address->button);
    return busMessage;
}
