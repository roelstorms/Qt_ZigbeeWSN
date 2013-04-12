
#include "libelchangefreqresponse.h"

LibelChangeFreqResponse::LibelChangeFreqResponse(std::vector<unsigned char> input) : ReceivePacket(input)
{
	if(getRFData().at(0) != 0X06)
	{
		std::cerr << "Tried to put a packed into a LibelChangeFreqResponse that was of the wrong type (see application ID != 0X06)" << std::endl;	
	}
}



bool LibelChangeFreqResponse::correspondsTo(LibelChangeFreqPacket * packet)
{
	if((getZigbee64BitAddress() == packet->getZigbee64BitAddress()))
	{
		std::vector<unsigned char> mask = getMask();
		std::vector<unsigned char> otherMask = packet->getMask();
		if((mask.at(0) = otherMask.at(0)) && (mask.at(1) = otherMask.at(1)))
		{
			return true;
		}	
	}

	return false;
}
 
