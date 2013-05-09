#ifndef WSPACKET_H
#define WSPACKET_H
#include <string>
#include "../errors.h"
#include "../packet.h"
#include <iostream>
#include "enums.h"
#include <boost/lexical_cast.hpp>
#include "XML/XML.h"

class WSPacket : public Packet
{
	public:
        WSPacket();

        PacketType getPacketType() = 0;
		~WSPacket();


};

#endif

