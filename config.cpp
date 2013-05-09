#include "config.h"
#include <iostream>
#include <fstream>


void Config::loadConfig(std::string configFileName)
{
    std::ifstream configFile;
    std::string line;
    configFile.open("configFileName.txt");
    while(getline(configFile, line))
    {
        int found = line.find("ipsumBaseURL");
        if(found != std::string::npos)
        {
            int equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                ipsumBaseURL = value;
            }
            found = std::string::npos;
        }


        found = line.find("personalKey");
        if(found != std::string::npos)
        {
            int equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                personalKey = value;
            }
        }

        found = line.find("dbName");
        if(found != std::string::npos)
        {
            int equalsPosition = line.find("=");
            if(equalsPosition != std::string::npos)
            {
                std::string value = line.substr(equalsPosition + 1);
                value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());
                dbName = value;
            }
        }
    }
}

