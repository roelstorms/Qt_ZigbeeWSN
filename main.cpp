/*
 *	Created by Roel Storms on 28/01/2013  
 *
 *  Main function for the gateway software. Here the config file is loaded, all tests are run and the MainClass is created.
 *
 *
 *  The ZigBee radio should be set in 9600 baud, API 2 mode and configured as coordinator.
 *
 *  TODO: Make a clean shutdown procedure (done, check once more)
 *  TODO: Free ZB packets at the right moment
 *  TODO: Resending of packets
 *
 */

#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "mainclass.h"
#include "testclass.h"

void init();

int main(int argc, char* argv[])
{
    std::cout << "argv[1]: " << argv[1] << std::endl;
    if(argc != 2)
    {
        std::cerr << "also provide the path to config file" << std::endl;
        return 1;
    }

    if(!Config::loadConfig((argv[1])))
    {
        std::cout << "Could not load config file" << std::endl;
        return 1;
    }

    TestClass test;
    test.runAll();


/*
    std::map<int, bool> sensors;
    sensors.insert(std::pair<int, bool> (0, true));
    sensors.insert(std::pair<int, bool> (0, true));
    sensors.insert(std::pair<int, bool> (0, true));
    sensors.insert(std::pair<int, bool> (0, true));

    IpsumChangeInUsePacket changeInUsePacket(0,0, sensors, true);
    std::cout << "Ipsum change freq packet size: " << sizeof(changeInUsePacket) << std::endl;

    std::cout << "Main" << std::endl;


    int user = getuid();

    if (user != 0)
    {
        std::cerr << "root privileges needed" << std::endl;
        return 1;
    }


    try
    {
        MainClass mainClass(argc, argv, 100, 3);
        mainClass();
    }
    catch(StartupError)
    {
        std::cerr << "error at startup" << std::endl;
        return 1;
    }

*/

	return 0;
}


