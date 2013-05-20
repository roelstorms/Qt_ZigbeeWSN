#ifndef TRANSMITSTATUSPACKET_H
#define TRANSMITSTATUSPACKET_H
#include "incomingpacket.h"
#include "errors.h"

class TransmitStatusPacket : public IncomingPacket
{
public:
    TransmitStatusPacket(std::vector<unsigned char> input) throw (InvalidPacketType);
    PacketType getPacketType(){ return ZB_TRANSMIT_STATUS; };

    unsigned char getFrameID() const;

    /*
     *  returns delivery status, if this is 0 then delivery was succesfull. For all other cases, check the datasheet.
     */
    unsigned char getDeliveryStatus() const;

    /*
     *  returns discovery status, if this is 0 then no discovery had to be done. For all other cases, check the datasheet
     */
    unsigned char getDiscoveryStatus() const; //
};

#endif // TRANSMITSTATUSPACKET_H
