#include "wspacket.h"

WSPacket::WSPacket() : Packet()
{
    std::cout << "WSPacket constructor" << std::endl;
}

WSPacket::~WSPacket()
{

}

SensorType WSPacket::stringToSensorType(std::string sensorType) throw (InvalidWSXML)
{
    std::cout << "stringToSensorType" << std::endl;
    if(sensorType == "zigbeeTemp")
    {
        return TEMP;
    }
    else if (sensorType == "zigbeeHum")
    {
        return HUM;
    }
    else if (sensorType == "zigbeePres")
    {
        return PRES;
    }
    else if (sensorType == "zigbeeBat")
    {
        return BAT;
    }
    else if (sensorType == "zigbeeCO2")
    {
        return CO2;
    }
    else if (sensorType == "zigbeeAnemo")
    {
        return ANEMO;
    }
    else if (sensorType == "zigbeeVane")
    {
        return VANE;
    }
    else if (sensorType == "zigbeePluvio")
    {
        return PLUVIO;
    }
    else
    {
        throw InvalidWSXML();
    }
}
