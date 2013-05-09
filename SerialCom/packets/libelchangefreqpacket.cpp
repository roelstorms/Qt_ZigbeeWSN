#include "libelchangefreqpacket.h"

LibelChangeFreqPacket::LibelChangeFreqPacket(std::vector<unsigned char> zigbeeAddress64bit, std::vector<std::pair<SensorType, int>> newFrequencies, unsigned char frameID) : TransmitRequestPacket()
{
    std::vector<unsigned char> data;
    int mask = 0;

	for(auto it = newFrequencies.begin(); it < newFrequencies.end(); ++it)
	{
		switch((*it).first)
		{
			case TEMP:
				mask += 0X01;
			break;	
			case HUM:
				mask += 0X02;
			break;	
			case PRES:
				mask += 0X04;
			break;	
			case BAT:
				mask += 0X08;
			break;	
			case CO2:
				mask += 0X10;
			break;	
			case ANEMO:
				mask += 0X20;
			break;	
			case VANE:
				mask += 0X40;
			break;	
			case PLUVIO:
				mask += 0X80;
			break;	
		}
	}
	std::cout << "calculated mask in libelchangefreqpacket constructor: " << mask << std::endl;	
    data.push_back(mask / 256);
    data.push_back(mask % 256);

	for(auto it = newFrequencies.begin(); it < newFrequencies.end(); ++it)
	{
        data.push_back((*it).second / 256);
        data.push_back((*it).second % 256);
	}
        setData(0x07, zigbeeAddress64bit, data, frameID);
}
