#ifndef WSADDSENSORSPACKET_H
#define WSADDSENSORSPACKET_H

#include "wspacket.h"


class WSAddSensorsPacket : public WSPacket
{
private:

    int sensorGroupID;
    std::map<SensorType, int> sensors;

public:
    WSAddSensorsPacket(std::string data) throw (InvalidXMLError);

    PacketType getPacketType(){ return WS_ADD_SENSORS_COMMAND; };

    const int& getSensorGroupID() const;
    const std::map<SensorType, int>& getSensors() const;

    SensorType stringToSensorType(std::string sensorType) throw (InvalidWSXML);
};

#endif // WSADDSENSORSPACKET_H
