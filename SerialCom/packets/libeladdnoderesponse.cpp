
#include "libeladdnoderesponse.h"

LibelAddNodeResponse::LibelAddNodeResponse(std::vector<unsigned char> input) : ReceivePacket(input)
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


        std::vector<bool> mask;
        unsigned int maskChars = getMask().at(0) * 256 + getMask().at(1);
        for(int i = 0; i < 16; ++i)
        {
            mask.push_back(maskChars & 0x0001);
            maskChars = maskChars >> 1;
        }

        auto data = getData();
        if(mask.at(0) == 1)
        {
            sensors.push_back(TEMP);
        }
        if(mask.at(1) == 1)
        {
            sensors.push_back(HUM);
        }
        if(mask.at(2) == 1)
        {
            sensors.push_back(PRES);
        }
        if(mask.at(3) == 1)
        {
            sensors.push_back(BAT);
        }
        if(mask.at(4) == 1)
        {
            sensors.push_back(CO2);
        }
        if(mask.at(5) == 1)
        {
            sensors.push_back(ANEMO);
        }
        if(mask.at(6) == 1)
        {
            sensors.push_back(VANE);
        }
        if(mask.at(7) == 1)
        {
            sensors.push_back(PLUVIO);
        }

         std::cerr << "Invalid LibelAddNodeResponse packet structure, see constructor" << std::endl;
    }
}

const std::vector<SensorType> &LibelAddNodeResponse::getSensors() const
{
    return sensors;
}



bool LibelAddNodeResponse::correspondsTo(LibelAddNodePacket * packet)
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
 
