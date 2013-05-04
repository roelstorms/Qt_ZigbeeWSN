#ifndef WSPACKET_H
#define WSPACKET_H
#include <string>
#include "../errors.h"
#include "../packet.h"
#include <iostream>
#include "enums.h"
#include <boost/lexical_cast.hpp>
#include "XML/XML.h"

enum RequestType
{
	CHANGE_FREQUENCY, ADD_NODE, ADD_SENSOR, REQUEST_DATA
};

class WSPacket : public Packet
{
	private:
		RequestType requestType;
		std::string data;
	public:
        WSPacket();

        PacketType getPacketType() = 0;
		~WSPacket();

        SensorType stringToSensorType(std::string sensorType) throw (InvalidWSXML);
};

#endif

