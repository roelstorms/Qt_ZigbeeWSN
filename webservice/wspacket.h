#ifndef WSPACKET_H
#define WSPACKET_H
#include <string>
#include "../errors.h"
#include "../packet.h"
#include <iostream>

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
		WSPacket(std::string aURI, std::string aData) throw (WebserviceInvalidCommand);
		~WSPacket();
		const RequestType& getRequestType() const;
		const std::string& getRequestData() const;

		PacketType getPacketType(){ return WS_COMMAND; };

};

#endif

