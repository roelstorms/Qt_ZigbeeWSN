#ifndef LIBELCHANGENODEFREQ_H
#define LIBELCHANGENODEFREQ_H

#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>

#include "../../errors.h"
#include "transmitrequestpacket.h"
#include "../../enums.h"



class LibelChangeNodeFreqPacket : public TransmitRequestPacket 
{
	private:
	public:
		
        LibelChangeNodeFreqPacket(std::vector<unsigned char> zigbeeAddress64bit, int newFrequency, unsigned char frameID = 0);
	
		PacketType getPacketType(){ return ZB_LIBEL_CHANGE_NODE_FREQ; };

		
};

#endif


