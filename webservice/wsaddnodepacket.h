#ifndef WSADDNODEPACKET_H
#define WSADDNODEPACKET_H

#include "wspacket.h"
#include <boost/lexical_cast.hpp>

class WSAddNodePacket : public WSPacket
{
private:
    int installationID, sensorGroupID;
    std::string zigbee64BitAddress;

public:
    WSAddNodePacket(std::string data);


    PacketType getPacketType(){ return WS_ADD_NODE_COMMAND; };

    const int& getInstallationID() const;
    const int& getSensorGroupID() const;
    const std::string& getZigbeeAddress64Bit() const;
};

#endif // WSCHANGEINUSEPACKET_H
