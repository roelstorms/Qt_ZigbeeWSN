#ifndef LIBELCHANGEFREQ_H
#define LIBELCHANGEFREQ_H

#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>

#include "../../errors.h"
#include "transmitrequestpacket.h"
#include "../../enums.h"



class LibelChangeFreqPacket : public TransmitRequestPacket 
{
	private:
	public:
		
		LibelChangeFreqPacket(std::vector<unsigned char> destinationZB64bitAddress, std::vector<std::pair<SensorType, int>> newFrequencies);
	
		PacketType getPacketType(){ return ZB_LIBEL_CHANGE_FREQ; };

		
};

#endif


