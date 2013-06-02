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
    testConfig();
    testWSPackets();
    testXML();
    testLibelChangeFreqPacket();
    testTransmitStatusPacket();
    testSQL();
}

void TestClass::testConfig()
{

    assertTest(Config::getDbName() == "../zigbee.dbs" , "DB name loading");
    assertTest(Config::getIpsumBaseURL() == "http://ipsum.groept.be" , "Ipsum url loading");
    assertTest(Config::getNumberOfRetries() == 2 , "Number of retries loading");
    assertTest(Config::getExpirationTime() == 15 , "Expiration time loading");
    assertTest(Config::getPersonalKey() == "a31dd4f1-9169-4475-b316-764e1e737653" , "Ipsum personal key loading");
    assertTest(Config::getXBeeBaudRate() == 9600 , "XBee baud rate loading");
    assertTest(Config::getXBeePortNumber() == 0, "XBee port number loading");
}

void TestClass::testWSPackets()
{
    {
        std::string input("<addNode>\n<installationID>32</installationID>\n<sensorGroupID>546</sensorGroupID>\n<zigbeeAddress>0013A2004069737C</zigbeeAddress>\n</addNode>");
        WSAddNodePacket p(input);
        assertTest(p.getInstallationID() == 32 , "Installation ID of WSAddNodePacket");
        assertTest(p.getSensorGroupID() == 546 , "SensorGroupID ID of WSAddNodePacket");
        assertTest(p.getZigbeeAddress64Bit() == "0013A2004069737C" , "ZigbeeAddress of WSAddNodePacket");
    }
}

void TestClass::testXML()
{
    XML xml;

    {   // XML login test scope
        std::string expected = "<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"no\" ?>\n<UserLogin>\n\n  <username>username</username>\n\n  <password>password</password>\n\n</UserLogin>\n";
        assertTest(expected.compare(xml.login("username", "password")) == 0 , "XML login check");
    }

    {   // XML analyze login test scope
        bool exception = false;
        try
        {
            xml.analyzeLoginReply("<Credentials><token>3694b670-d3d5-4505-99a2-e27e22b1afe0</token><error>false</error><expire>2012-07-02T08:16:38.5415621Z</expire><user>20</user></Credentials>");
        }
        catch(...)
        {
            std::cout << "invalid xml error" << std::endl;
            exception = true;
        }
        assertTest(!exception , "XML analyseLoginReply throws exception");
    }

    {   // Pure for a memory leak check on this function

        std::string serializedXML = xml.serializeDOM(xml.parseToDom("<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"no\" ?>\n<UserLogin>\n\n  <username>username</username>\n\n  <password>password</password>\n\n</UserLogin>\n"));
        std::cout << "serializedXML: " << std::endl << serializedXML << std::endl;
    }

    std::cout << "end of XML test" << std::endl;

}

void TestClass::testSQL()
{
    Sql sql;
    std::vector<unsigned char> input{0x7E, 0x00, 0x20, 0x90, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xFF, 0xFE, 0X01, 0x01, 0x0A, 0x01, 0x23, 0x02, 0xFF, 0xFE, 0xFF, 0xFE, 0x07, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0x8E};
    LibelIOPacket libelIOPacket(input);
    sql.addMeasurement(&libelIOPacket);


    // IMPORTANT: If one of the tests below fails, check if the DB doesn't contain a node with 126543 as address
    sql.deleteNode("126543");

    assertTest(sql.getNodeID("126543") == -1  , "Check if sql.getNodeID returns -1 when node doesn't exist in DB");
    assertTest(sql.makeNewNode(15,15, "126543"), "Should return true since this node shouldn't exist yet");   // Should be false since this node doesn't exist and this one was added.
    assertTest(sql.makeNewNode(15,15, "126543") == false, "Should return false since this node should exist yet");   // Should be false since this node doesn't exist and this one was added.
    assertTest(sql.getNodeID("126543") == 15  , "Check if sql.getNodeID returns node id when node exists in DB");
    assertTest(sql.deleteNode("126543")  , "Check if a node can be deleted from DB");
}

void TestClass::testLibelChangeFreqPacket()
{
    std::vector<unsigned char> zigbeeAddress64bit{0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77};
    std::vector<std::pair<SensorType, int> > newFrequencies;
    newFrequencies.push_back(std::pair<SensorType, int>(TEMP, 60));
    newFrequencies.push_back(std::pair<SensorType, int>(LUMINOSITY, 10));
    newFrequencies.push_back(std::pair<SensorType, int>(PRES, 20));
    newFrequencies.push_back(std::pair<SensorType, int>(CO2, 300));
    LibelChangeFreqPacket libelChangeFreqPacket(zigbeeAddress64bit, newFrequencies, 3);
    std::vector<unsigned char> expected{0X7E, 0X00, 0X24, 0X10, 0X03, 0X77, 0X77, 0X77, 0X77, 0X77, 0X77, 0X77, 0X77, 0XFF, 0XFE, 0X00, 0X00, 0X07, 0X01, 0X23, 0X01, 0X77, 0X77, 0X77, 0X77, 0X77, 0X77, 0X77, 0X77, 0X01, 0X15, 0X00, 0X06, 0X00, 0X01, 0X00, 0X02, 0X00, 0X1E, 0X16};
    assertTest(expected == libelChangeFreqPacket.getEncodedPacket(), "Check LibelChangeFreqPacket");
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

void TestClass::testErrorPacket()
{
    //7E 00 13 90 00 13 A2 00 40 69 73 77 02 F3 01 0C 01 23 00 02 F3 0A 02
    std::vector<unsigned char> input{0X7E, 0X00, 0X13, 0X90, 0X00, 0X13, 0XA2, 0X00, 0X40, 0X69, 0X73, 0X77, 0X02, 0XF3, 0X01, 0X01, 0X0C, 0X01, 0X23, 0X00, 0X02, 0XF3, 0X0A, 0X02};
    LibelErrorPacket libelErrorPacket(input);
    std::cout << "Error msg : " << libelErrorPacket.getErrorMsg() << std::endl;
}


