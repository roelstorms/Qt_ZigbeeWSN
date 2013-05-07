
#include "libelchangefreqresponse.h"

LibelChangeFreqResponse::LibelChangeFreqResponse(std::vector<unsigned char> input): ReceivePacket(input)
{
    try
    {
        if(getRFData().at(0) != 0X08)
        {
            std::cerr << "Tried to put a packed into a LibelChangeFreqResponse that was of the wrong type (see application ID != 0X06)" << std::endl;
        }

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
            sensorFrequencies.insert(std::pair<SensorType, int>(TEMP, data.at(0)));
        }
        if(mask.at(1) == 1)
        {
            sensorFrequencies.insert(std::pair<SensorType, int>(HUM, data.at(1)));
        }
        if(mask.at(2) == 1)
        {
            sensorFrequencies.insert(std::pair<SensorType, int>(PRES, data.at(2)));
        }
        if(mask.at(3) == 1)
        {
            sensorFrequencies.insert(std::pair<SensorType, int>(BAT, data.at(3)));
        }
        if(mask.at(4) == 1)
        {
            sensorFrequencies.insert(std::pair<SensorType, int>(CO2, data.at(4)));
        }
        if(mask.at(5) == 1)
        {
            sensorFrequencies.insert(std::pair<SensorType, int>(ANEMO, data.at(5)));
        }
        if(mask.at(6) == 1)
        {
            sensorFrequencies.insert(std::pair<SensorType, int>(VANE, data.at(6)));
        }
        if(mask.at(7) == 1)
        {
            sensorFrequencies.insert(std::pair<SensorType, int>(PLUVIO, data.at(7)));
        }
    }
    catch(...)
    {
        std::cerr << "Invalid LibelChangeFreqResponse packet structure, see constructor" << std::endl;
    }
}

const std::map<SensorType, int>& LibelChangeFreqResponse::getFrequencies() const
{
    return sensorFrequencies;
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
 
