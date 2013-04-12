#ifndef LIBELMASKRESPONSE_H 
#define LIBELMASKRESPONSE_H


#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include "receivepacket.h"
#include <iomanip>
#include <map>
#include "../../errors.h"
#include "../../enums.h"
#include "libelmaskrequest.h"

class LibelMaskResponse : public ReceivePacket 
{
	private:
	public:
		LibelMaskResponse(std::vector<unsigned char> input);
		PacketType getPacketType(){ return ZB_LIBEL_MASK_RESPONSE; };
		bool correspondsTo(LibelMaskRequest * packet);
};

#endif


