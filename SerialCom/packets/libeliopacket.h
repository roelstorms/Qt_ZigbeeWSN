#ifndef LIBELIOPACKET_H
#define LIBELIOPACKET_H

#define CREATESENSORGETTER(name, sensortype) float name() const throw(DataNotAvailable)\
{\
	auto it = sensorData.find(sensortype); \
	if(it == sensorData.end())\
	{\
		std::cout << "sensortype of unfound data: " << sensortype << std::endl;\
		throw DataNotAvailable(); \
	}\
	return it->second;\
}




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
		LibelIOPacket(std::vector<unsigned char> input);
		PacketType getPacketType(){ return ZB_LIBEL_IO; };
		const std::map<SensorType, float>& getSensorData() const;

		CREATESENSORGETTER(getTemperature, TEMP)		//Macro to generate setters for all the sensor types	
		CREATESENSORGETTER(getHumidity, HUM)	
		CREATESENSORGETTER(getPressure, PRES)	
		CREATESENSORGETTER(getBattery, BAT)	
		CREATESENSORGETTER(getCO2, CO2)	
		CREATESENSORGETTER(getAnemo, ANEMO)	
		CREATESENSORGETTER(getVane, VANE)
		CREATESENSORGETTER(getPluvio, PLUVIO)



};

#endif


