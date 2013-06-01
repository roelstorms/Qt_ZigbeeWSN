#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <map>
#include "enums.h"

class Config
{
    private:
        static std::string ipsumBaseURL, personalKey, dbName;
        static int numberOfRetries, expirationTime, xbeePortNumber, xbeeBaudRate;

        static std::map<SensorType, std::string> sensorMap;

    public:
        static bool loadConfig(std::string configFileName);

        static const std::string &getIpsumBaseURL();
        static const std::string &getPersonalKey() ;
        static const std::string &getDbName() ;
        static const int &getNumberOfRetries() ;
        static const std::map<SensorType, std::string> &getSensorMap() ;
        static const int &getExpirationTime();
        static const int &getXBeeBaudRate();
        static const int &getXBeePortNumber();
};



#endif // CONFIG_H
