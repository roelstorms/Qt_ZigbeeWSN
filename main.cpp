/*
 *	Created by Roel Storms on 28/01/2013  
 *
 *	TODO: Saving url and XML of packet that should be sent to ipsum to the sql database whenever ipsum is down
 *
 */


#include "mainclass.h"
#include "testclass.h"

int main(int argc, char* argv[])
{
    TestClass test;
    test.testTransmitStatusPacket();

    std::cout << "Main" << std::endl;

    int user = getuid();
    if (user != 0)
    {
        std::cerr << "root privileges needed" << std::endl;
        throw NoRoot();
    }

    try
    {
//        MainClass mainClass(argc, argv, 6000);
//        mainClass();
    }
    catch(StartupError)
    {
        std::cerr << "error at startup" << std::endl;
        return 1;
    }



	return 0;
}

