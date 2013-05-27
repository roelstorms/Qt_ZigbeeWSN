#ifndef LIBELIOPACKET_H
#define LIBELIOPACKET_H



#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include "receivepacket.h"
#include <iomanip>
#include <map>
#include "../../errors.h"
#include "receivepacket.h"
#include "../../enums.h"


class LibelIOPacket : public ReceivePacket 
{
	private:
		std::map<SensorType, float> sensorData;
	public:
        /*
         *  Sample input for this constuctor: 0x7E, 0x00, 0x20, 0x90, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xFF, 0xFE, 0X01, 0x01, 0x0A, 0x01, 0x23, 0x02, 0xFF, 0xFE, 0xFF, 0xFE, 0x07, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0x8E
         *  This is a package with TEMP, HUM, PRES from 0040404040404040
         */
		LibelIOPacket(std::vector<unsigned char> input);
		PacketType getPacketType(){ return ZB_LIBEL_IO; };
		const std::map<SensorType, float>& getSensorData() const;

        float getSensorData(SensorType sensorType);




};

#endif


