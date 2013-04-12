#include "sql.h"
#include "../testclass.h"
int main(int argc, char ** argv)
{
	Sql sql(std::string("zigbee.dbs"));
	std::cout << "main" << std::endl;
	std::cout << "threadsafe: " << sqlite3_threadsafe();
	std::cout << "output: " << std::endl << sql.updateSensorsInNode(132111, TEMP, 13) << std::endl;
	std::cout << sql.getNodeAddress(132111);
	TestClass::assert(sql.getNodeAddress(132111) == std::string("zigbee address"), "Testing Sql::getNodeAddress");
	auto sensors = sql.getSensorsFromNode(132111);
	auto it = sensors.find(TEMP);
	if(it != sensors.end())
	{
		std::cout << "sensorID of tempsensor of node 132111: " <<  it->second << std::endl;
		TestClass::assert(it->second == 13, "Checking sql.getSensorsFromNode | available sensors should give back their sensorID as stored in ipsum");
	}

	it = sensors.find(PLUVIO);
	TestClass::assert(it == sensors.end(), "Checking sql.getSensorsFromNode | not available sensors should not be in the returned map");

	it = sensors.find(VANE);
	TestClass::assert(it == sensors.end(), "Checking sql.getSensorsFromNode | sensors with wrong data as sensorID should not be returned");
}
