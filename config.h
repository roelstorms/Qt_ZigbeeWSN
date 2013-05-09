#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <algorithm>

class Config
{
    public:
        static std::string ipsumBaseURL;
        static std::string personalKey;
        static std::string dbName;
        static void loadConfig(std::string configFileName);


};



#endif // CONFIG_H
