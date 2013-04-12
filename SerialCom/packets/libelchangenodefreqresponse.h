#ifndef LIBELCHANGENODEFREQRESPONSE_H 
#define LIBELCHANGENODEFREQRESPONSE_H


#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include "receivepacket.h"
#include <iomanip>
#include <map>
#include "../../errors.h"
#include "../../enums.h"
#include "libelchangenodefreqpacket.h"

class LibelChangeNodeFreqResponse : public ReceivePacket 
{
	private:
	public:
		LibelChangeNodeFreqResponse(std::vector<unsigned char> input);
		PacketType getPacketType(){ return ZB_LIBEL_CHANGE_NODE_FREQ_RESPONSE; };
		bool correspondsTo(LibelChangeNodeFreqPacket * packet);
};

#endif


