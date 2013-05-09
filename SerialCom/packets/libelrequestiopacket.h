#ifndef LIBELREQUESTIOPACKET_H
#define LIBELREQUESTIOPACKET_H

#include "transmitrequestpacket.h"
#include "../../enums.h"
#include "../../errors.h"

class LibelRequestIOPacket : public TransmitRequestPacket 
{
	private:
	
	public:
    LibelRequestIOPacket(std::vector<unsigned char> zigbeeAddress64bit, std::vector<SensorType> sensors, unsigned char frameID = 0);

	PacketType getPacketType(){ return ZB_LIBEL_REQUEST_IO; };
};

#endif
