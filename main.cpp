/*
 *	Created by Roel Storms on 28/01/2013  
 *
 *	TODO: Saving url and XML of packet that should be sent to ipsum to the sql database whenever ipsum is down
 *
 *  To know how much memory this proces is consuming use : sudo cat /proc/<PID>/smaps
 *  This can give an indication if you will have trouble running it on an embedded device. For now the memory used just after startup is less then 1MB.
 *  Has to be rechecked when program is running for a few houres and recieves some traffic.
 */
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "mainclass.h"
#include "testclass.h"

int main(int argc, char* argv[])
{
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
        MainClass mainClass(argc, argv, 6000, 3);
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

