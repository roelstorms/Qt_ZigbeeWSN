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
    testSQL();
}

void TestClass::testXML()
{
    XML xml;

    std::string expected = "<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"no\" ?>\n<UserLogin>\n\n  <username>username</username>\n\n  <password>password</password>\n\n</UserLogin>\n";
    assertTest(expected.compare(xml.login("username", "password")) == 0 , "XML login check");

    /*
    try
    {
        xml.analyzeLoginReply("<Credentials><token>3694b670-d3d5-4505-99a2-e27e22b1afe0</token><error>false</error><expire>2012-07-02T08:16:38.5415621Z</expire><user>20</user></Credentials>");
    }
    catch(...)
    {
        std::cout << "invalid xml error" << std::endl;
    }
    std::cout << "end of XML test" << std::endl;
    */
}

void TestClass::testSQL()
{
    Sql sql("../zigbee.dbs");
    std::vector<unsigned char> input{0x7E, 0x00, 0x20, 0x90, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xFF, 0xFE, 0X01, 0x01, 0x0A, 0x01, 0x23, 0x02, 0xFF, 0xFE, 0xFF, 0xFE, 0x07, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0x8E};
    LibelIOPacket libelIOPacket(input);
    sql.addMeasurement(libelIOPacket);
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


