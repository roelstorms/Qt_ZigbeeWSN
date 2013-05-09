
#include "libelchangenodefreqresponse.h"

LibelChangeNodeFreqResponse::LibelChangeNodeFreqResponse(std::vector<unsigned char> input) : ReceivePacket(input)
{
    try
    {
        if(getRFData().at(0) != 0X06)
        {
            std::cerr << "Tried to put a packed into a LibelChangeNodeFreqResponse that was of the wrong type (see application ID != 0X08)" << std::endl;
        }
    }
    catch(...)
    {
        std::cerr << "Invalid LibelChangeNodeFreqResponse packet structure, see constructor" << std::endl;

    }
}



bool LibelChangeNodeFreqResponse::correspondsTo(LibelChangeNodeFreqPacket * packet)
{
    if((getZigbee64BitAddress() == packet->getZigbee64BitAddress()) && (getFrameID() == packet->getFrameID()))
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
 
