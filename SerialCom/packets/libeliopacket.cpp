#include "libeliopacket.h"

LibelIOPacket::LibelIOPacket(std::vector<unsigned char> input) : ReceivePacket(input)
{
    try
    {
        auto rfData = getRFData();
        if(rfData.at(0) != 0X0A)
        {
            std::cerr << "Tried to put a packed into a LibelIOPacket that was of the wrong type (see application ID != 0X0A)" << std::endl;
        }

        std::cout << "LibelIOPacket constructor" << std::endl;
        std::cout << "rfData length: " << rfData.size() << std::endl;

        std::vector<bool> mask;

        auto data = getData();
        std::cout << "data length: " << (int) data.size() << std::endl;
        unsigned int maskChars = getMask().at(0) * 256 + getMask().at(1);
        for(int i = 0; i < 16; ++i)
        {
            mask.push_back(maskChars & 0x0001);
            maskChars = maskChars >> 1;
        }


        std::cout << "mask: " << std::endl;
    #ifdef PACKET_DEBUG
        for(auto it = mask.begin(); it < mask.end(); ++it)
        {
            std::cout << (*it) << std::endl;
        }
    #endif
        std::cout << std::endl << std::endl;
        int count = 3;
        float value;
        if(mask.at(0) == 1)
        {
            value = data.at(count++) * 256 ;
            value += data.at(count++);
            value /= 100;
            value -= 40;
            sensorData.insert(std::pair<SensorType, float>(TEMP, value));
            std::cout << "Temp data found in packet: " << value << std::endl;
            value = 0;
        }

        if(mask.at(1) == 1)
        {
            value = data.at(count++);
            sensorData.insert(std::pair<SensorType, float>(HUM, value));
            std::cout << "Humidity data found in packet: " << value << std::endl;
            value = 0;
        }

        if(mask.at(2) == 1)
        {
            value = data.at(count++) * 256 ;
            value += data.at(count++);
            value /= 10;
            sensorData.insert(std::pair<SensorType, float>(PRES, value));
            std::cout << "Pressure data found in packet: " << value << std::endl;
            value = 0;
        }

        if(mask.at(3) == 1)
        {
            value = data.at(count++);
            sensorData.insert(std::pair<SensorType, float>(BAT, value));
            std::cout << "Battery data found in packet: " << value << std::endl;
            value = 0;

        }
        if(mask.at(4) == 1)
        {
            value = data.at(count++) * 256 ;
            value += data.at(count++);
            sensorData.insert(std::pair<SensorType, float>(CO2, value));
            std::cout << "CO2 data found in packet: " << value << std::endl;
            value = 0;
        }

        if(mask.at(5) == 1)
        {
            value += data.at(count++);
            sensorData.insert(std::pair<SensorType, float>(ANEMO, value));
            std::cout << "Anemo data found in packet: " << value << std::endl;
            value = 0;
        }

        if(mask.at(6) == 1)
        {
            value += data.at(count++);
            sensorData.insert(std::pair<SensorType, float>(VANE, value));
            std::cout << "Vane data found in packet: " << value << std::endl;
            value = 0;
        }

        if(mask.at(7) == 1)
        {
            value = data.at(count++) * 256 ;
            value += data.at(count++);
            value = value * 0.2794;      // Conversion from time the bucket is full to mm of rain
            sensorData.insert(std::pair<SensorType, float>(PLUVIO, value));
            std::cout << "Pluvio data found in packet: " << value << std::endl;
            value = 0;
        }

        if(mask.at(8) == 1)
        {
            value += data.at(count++);
            sensorData.insert(std::pair<SensorType, float>(LUMINOSITY, value));
            std::cout << "LUMINOSITY data found in packet: " << value << std::endl;
            value = 0;
        }

        if(mask.at(9) == 1)
        {
            value = data.at(count++) * 256 ;
            value += data.at(count++);
            sensorData.insert(std::pair<SensorType, float>(SOLAR_RAD, value));
            std::cout << "SOLAR_RAD data found in packet: " << value << std::endl;
            value = 0;
        }
    }
    catch(...)
    {
         std::cerr << "Invalid LibelIOPacket packet structure, see constructor" << std::endl;
    }
}


const std::map<SensorType, float>& LibelIOPacket::  getSensorData() const
{
    return sensorData;
}

float LibelIOPacket::getSensorData(SensorType sensorType)
{
    auto it = sensorData.find(sensorType);
    if(it == sensorData.end())
    {
        std::cout << "sensortype of unfound data: " << sensorType << std::endl;
        throw DataNotAvailable();
    }
    return it->second;
}
