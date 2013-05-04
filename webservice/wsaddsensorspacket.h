#ifndef WSADDSENSORSPACKET_H
#define WSADDSENSORSPACKET_H

#include "wspacket.h"


class WSAddSensorsPacket : public WSPacket
{
private:

    int sensorGroupID;
    std::map<SensorType, int> sensors;

public:
    WSAddSensorsPacket(std::string data);

    PacketType getPacketType(){ return WS_ADD_SENSORS_COMMAND; };

    const int& getSensorGroupID() const;
    const std::map<SensorType, int>& getSensors() const;
};

#endif // WSADDSENSORSPACKET_H
