
#include "libelmaskresponse.h"

LibelMaskResponse::LibelMaskResponse(std::vector<unsigned char> input) : ReceivePacket(input)
{
    try
    {
        if(getRFData().at(0) != 0X02)
        {
            std::cerr << "Tried to put a packed into a LibelAddNodeResponse that was of the wrong type (see application ID != 0X02)" << std::endl;
        }
    }
    catch(...)
    {
        std::cerr << "Invalid LibelMaskResponse packet structure, see constructor" << std::endl;
    }
}



bool LibelMaskResponse::correspondsTo(LibelMaskRequest * packet)
{
    if((getZigbee64BitAddress() == packet->getZigbee64BitAddress()) && (getFrameID() == packet->getFrameID()))
	{
		return true;	
	}

	return false;
}
 
