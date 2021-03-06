#include "config.h"
#include <iostream>
#include <fstream>

std::string Config::ipsumBaseURL = "";
std::string Config::personalKey = "";
std::string Config::dbName = "";
int Config::numberOfRetries = 0;
int Config::expirationTime = 0;
std::map<SensorType, std::string> Config::sensorMap;
int Config::xbeeBaudRate = 0;
int Config::xbeePortNumber = -1;    // Set to -1 because failing to load is easier to check since port most often is 0.

bool Config::loadConfig(std::string configFileName)
{
    std::ifstream configFile(configFileName);
    std::string line;
    if (!configFile.is_open())
    {
        return false;
    }
    while(getline(configFile, line))
    {
        size_t found = line.find("ipsumBaseURL");
        if(found != std::string::npos)
        {
            size_t equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                Config::ipsumBaseURL = value;
            }
            found = std::string::npos;
        }


        found = line.find("personalKey");
        if(found != std::string::npos)
        {
            size_t equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                Config::personalKey = value;
            }
        }

        found = line.find("dbName");
        if(found != std::string::npos)
        {
            size_t equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                Config::dbName = value;
            }
        }

        found = line.find("numberOfRetries");
        if(found != std::string::npos)
        {
            size_t equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                Config::numberOfRetries = boost::lexical_cast<int>(value);
            }
        }

        found = line.find("expirationTime");
        if(found != std::string::npos)
        {
            size_t equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                Config::expirationTime = boost::lexical_cast<int>(value);
            }
        }

        found = line.find("xbeePortNumber");
        if(found != std::string::npos)
        {
            size_t equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                Config::xbeePortNumber = boost::lexical_cast<int>(value);
            }
        }

        found = line.find("xbeeBaudRate");
        if(found != std::string::npos)
        {
            size_t equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                Config::xbeeBaudRate = boost::lexical_cast<int>(value);
            }
        }
    }

    Config::sensorMap.insert(std::pair<SensorType, std::string> (TEMP, "temperature"));
    Config::sensorMap.insert(std::pair<SensorType, std::string> (HUM, "humidity"));
    Config::sensorMap.insert(std::pair<SensorType, std::string> (PRES, "pressure"));
    Config::sensorMap.insert(std::pair<SensorType, std::string> (BAT, "battery"));
    Config::sensorMap.insert(std::pair<SensorType, std::string> (CO2, "CO2"));
    Config::sensorMap.insert(std::pair<SensorType, std::string> (ANEMO, "anemo"));
    Config::sensorMap.insert(std::pair<SensorType, std::string> (VANE, "vane"));
    Config::sensorMap.insert(std::pair<SensorType, std::string> (PLUVIO, "pluvio"));
    Config::sensorMap.insert(std::pair<SensorType, std::string> (LUMINOSITY, "luminosity"));
    Config::sensorMap.insert(std::pair<SensorType, std::string> (SOLAR_RAD, "solar_rad"));

    #ifdef CONFIG_DEBUG
        std::cout << "dbName : " << dbName << std::endl;
    #endif

    return true;
}

const std::string& Config::getIpsumBaseURL()
{
    return Config::ipsumBaseURL;
}

const std::string &Config::getPersonalKey()
{
    return Config::personalKey;
}

const std::string &Config::getDbName()
{
    return Config::dbName;
}

const int &Config::getNumberOfRetries()
{
    return Config::numberOfRetries;
}

const std::map<SensorType, std::string> &Config::getSensorMap()
{
    return Config::sensorMap;
}

const int &Config::getExpirationTime()
{
    return Config::expirationTime;
}

const int &Config::getXBeeBaudRate()
{
    return xbeeBaudRate;
}

const int &Config::getXBeePortNumber()
{
    return xbeePortNumber;
}


