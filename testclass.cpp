#include "testclass.h"


void TestClass::assertTest(bool condition, std::string message)
{

    if(condition)
    {
        std::cout << message << "\033[1;32m succes\033[0m" << std::endl;
    }
    else
    {
        std::cout << message << "\033[1;31m failed\033[0m" << std::endl;
    }

}


void TestClass::runAll()
{
    testXML();
    testTransmitStatusPacket();
}

void TestClass::testXML()
{

}

void TestClass::testTransmitStatusPacket()
{
    {
        std::vector<unsigned char> input{0X7E, 0X00, 0X07, 0X8B, 0X02, 0X0E, 0X55, 0X00, 0X00, 0X00, 0X0F};
        TransmitStatusPacket packet(input);
        assertTest(packet.getDeliveryStatus() == 0X00, "transmitstatus packet delivery status check");
        assertTest(packet.getDiscoveryStatus() == 0X00, "transmitstatus packet discovery status check");
    }

    {
        std::vector<unsigned char> input{0X7E, 0X00, 0X07, 0X8B, 0X02, 0XFF, 0XFD, 0X28, 0X24, 0X02, 0X28};
        TransmitStatusPacket packet(input);
        assertTest(packet.getDeliveryStatus() == 0X24, "transmitstatus packet delivery status check");
        assertTest(packet.getDiscoveryStatus() == 0X02, "transmitstatus packet discovery status check");
    }
}


