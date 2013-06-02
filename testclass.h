/*
 *  Created by Roel Storms
 *
 *  In this class a lot of important or tricky functions are tested.
 *  These tests should always be run to check if some code is broken.
 *  In terminal a green succes or red failed is printed to indicate the result of these tests.
 *
 *  Loading the configs is tested but certain values are expected, if these values are changed
 *  in the config file, the tests will fail. This doesn't mean the function doesn't work.
 *
 */

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
    void testLibelChangeFreqPacket();
    void testTransmitStatusPacket();
    void testErrorPacket();


};


#endif
