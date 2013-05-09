#ifndef LIBELMASKREQUEST_H
#define LIBELMASKREQUEST_H

#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include "transmitrequestpacket.h"
#include <iomanip>
#include <map>
#include "../../errors.h"
#include "../../packet.h"
#include "../../enums.h"


class LibelMaskRequest : public TransmitRequestPacket 
{
	private:
	public:
		//LibelAddNodePacket(std::vector<unsigned char> input);
        LibelMaskRequest(std::vector<unsigned char> zigbeeAddress64bit, unsigned char frameID = 0);

		PacketType getPacketType(){ return ZB_LIBEL_MASK_REQUEST; };
};

#endif
