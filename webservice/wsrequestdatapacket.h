#ifndef WSREQUESTDATAPACKET_H
#define WSREQUESTDATAPACKET_H

#include "wspacket.h"


class WSRequestDataPacket : public WSPacket
{
private:
    int sensorGroupID;
    std::vector<int> sensors;
public:
    WSRequestDataPacket(std::string data);

    PacketType getPacketType(){ return WS_REQUEST_DATA_COMMAND; };

    const int& getSensorGroupID() const;
    const std::vector<int>& getSensors() const;
};

#endif // WSREQUESTIOPACKET_H
