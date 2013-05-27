#include "libelrequestiopacket.h"

LibelRequestIOPacket::LibelRequestIOPacket(std::vector<unsigned char> zigbeeAddress64bit, std::vector<SensorType> sensors, unsigned char frameID) : TransmitRequestPacket()
{
    std::vector<unsigned char> data;
    int mask = 0;

	for( auto it = sensors.begin(); it < sensors.end(); ++it)
	{
		switch(*it)
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
    data.push_back(mask / 256);
    data.push_back(mask % 256);
	
    setData(0x09, zigbeeAddress64bit, data, frameID);

}
