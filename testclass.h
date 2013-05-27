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
    void testXML();
    void testTransmitStatusPacket();
    void testSQL();
};


#endif
