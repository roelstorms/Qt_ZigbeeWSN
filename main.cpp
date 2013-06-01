/*
 *	Created by Roel Storms on 28/01/2013  
 *
 *  TODO: If parse to dom returns null program can crash. In add node on line 10 this is the case
 *
 *  TODO: Make a clean shutdown procedure
 *  TODO: Free ZB packets at the right moment
 *  TODO: Resending of packets
 *
 *  To know how much memory this proces is consuming use : sudo cat /proc/<PID>/smaps
 *  This can give an indication if you will have trouble running it on an embedded device. For now the memory used just after startup is less then 1MB.
 *  Has to be rechecked when program is running for a few houres and recieves some traffic.
 *
 */
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "mainclass.h"
#include "testclass.h"

void init();

int main(int argc, char* argv[])
{
   // init();
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
/*
void init()
{

    {
        sensorMap.insert(std::pair<SensorType, std::string> (TEMP, "temperature"));
        sensorMap.insert(std::pair<SensorType, std::string> (HUM, "humidity"));
        sensorMap.insert(std::pair<SensorType, std::string> (PRES, "pressure"));
        sensorMap.insert(std::pair<SensorType, std::string> (BAT, "battery"));
        sensorMap.insert(std::pair<SensorType, std::string> (CO2, "CO2"));
        sensorMap.insert(std::pair<SensorType, std::string> (ANEMO, "anemo"));
        sensorMap.insert(std::pair<SensorType, std::string> (VANE, "vane"));
        sensorMap.insert(std::pair<SensorType, std::string> (PLUVIO, "pluvio"));
        sensorMap.insert(std::pair<SensorType, std::string> (LUMINOSITY, "luminosity"));
        sensorMap.insert(std::pair<SensorType, std::string> (SOLAR_RAD, "solar_rad"));
    }


    {
        //std::cout << "sensormap" << std::endl;
    }




}
*/

