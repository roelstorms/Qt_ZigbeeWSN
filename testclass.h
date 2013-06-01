#ifndef TESTCLASS_H
#define TESTCLASS_H
#include <iostream>
#include <string>

#include "mainclass.h"

class TestClass
{
	private:


	public:
    void assertTest(bool condition, std::string message);

    void runAll();
    void testConfig();
    void testWSPackets();
    void testXML();
    void testSQL();
    void testTransmitStatusPacket();
    void testErrorPacket();

};


#endif
