#include "libelchangefreqpacket.h"

LibelChangeFreqPacket::LibelChangeFreqPacket(std::vector<unsigned char> zigbeeAddress64bit, std::vector<std::pair<SensorType, int> > newFrequencies, unsigned char frameID) : TransmitRequestPacket()
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
            case LUMINOSITY:
                mask += 0X100;
            break;
            case SOLAR_RAD:
                mask += 0X200;
            break;
		}
	}
	std::cout << "calculated mask in libelchangefreqpacket constructor: " << mask << std::endl;	
    data.push_back(mask / 256);
    data.push_back(mask % 256);

	for(auto it = newFrequencies.begin(); it < newFrequencies.end(); ++it)
	{
        int freq = it->second / 10;      // Divide by 10 since the ZigBee network works with 10s as a unit of time and the rest of the software uses 1s.
                                    // In the ZigBee network nodes have 16 bit processors this means that 2^16 seconds would mean less then a day.
                                    // On our computers an int is always 32 or 64 bit.
        data.push_back(freq / 256);
        data.push_back(freq % 256);
	}
        setData(0x07, zigbeeAddress64bit, data, frameID);
}
