#ifndef LIBELCHANGEFREQRESPONSE_H 
#define LIBELCHANGEFREQRESPONSE_H


#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include "receivepacket.h"
#include <iomanip>
#include <map>
#include "../../errors.h"
#include "../../enums.h"
#include "libelchangefreqpacket.h"

class LibelChangeFreqResponse : public ReceivePacket 
{
	private:
	public:
		LibelChangeFreqResponse(std::vector<unsigned char> input);
		PacketType getPacketType(){ return ZB_LIBEL_CHANGE_FREQ_RESPONSE; };
		bool correspondsTo(LibelChangeFreqPacket * packet);
};

#endif


