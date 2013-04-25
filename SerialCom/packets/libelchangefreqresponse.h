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
        std::map<SensorType, int> sensorFrequencies;
	public:
		LibelChangeFreqResponse(std::vector<unsigned char> input);

        const std::map<SensorType, int>& getFrequencies() const;

		PacketType getPacketType(){ return ZB_LIBEL_CHANGE_FREQ_RESPONSE; };
		bool correspondsTo(LibelChangeFreqPacket * packet);
};

#endif


