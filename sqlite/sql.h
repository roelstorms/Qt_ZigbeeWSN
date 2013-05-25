#ifndef SQL_H
#define SQL_H
#include "sqlite3.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "../enums.h"
#include "../errors.h"
#define BOOST_DISABLE_ASSERTS 	// If you do not define this then the boost assert macro will be included and its name collides with my TestClass::assert function 
#include <boost/lexical_cast.hpp>
#include <typeinfo>

class Sql
{
	private:
	sqlite3 *db;	
	std::vector<std::map<std::string, std::string>> selectReturn;
	public:
	Sql(std::string dbName);
	~Sql();
	static int callbackWrapper(void *thisPointer, int argc, char **argv, char **azColName);
	int callback(int argc, char **argv, char **azColName);
	std::vector<std::map<std::string, std::string>> executeQuery(std::string aQuery);

	

	void addIpsumPacket(const std::string& url, const std::string& XML);
	std::vector<std::map<std::string, std::string>> retrieveIpsumPacket();
	void removeIpsumPacket(int id);

	
	//Table nodes: 	nodeID (int), zigbee64bitaddress(text), zigbee16bitaddress(text), temperatureID(int), humidityID(int), pressureID(int),
	//		batteryID (int), co2ID(int), anemoID(int), pluvioID(int)
    std::string makeNewNode(int installationID, int nodeID, std::string zigbee64BitAddress)  throw (SqlError) ;
    std::string updateSensorsInNode(int nodeID, SensorType sensorType, int sensorID);
    std::string getNodeAddress(int nodeID) throw (SqlError);
    int getNodeID(std::string zigbeeAddress64Bit) throw (SqlError);
    int getInstallationID(std::string zigbeeAddress64Bit) throw (SqlError);
    std::map<SensorType, int> getSensorsFromNode(int nodeID) throw (SqlError);
};

#endif
