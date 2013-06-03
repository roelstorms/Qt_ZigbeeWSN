#include "sql.h"

Sql::Sql()
{
	int rc;
    std::cout << "Opening DB" << std::endl;
    rc = sqlite3_open(Config::getDbName().c_str(), &db);
	if( rc )
	{
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
	}
    std::cout << "DB opened" << std::endl;
}


Sql::~Sql()
{
	sqlite3_close(db);
}

int Sql::callbackWrapper(void *thisPointer, int argc, char **argv, char **azColName)
{

	return static_cast<Sql *>(thisPointer)->callback( argc,  argv, azColName);
}


int Sql::callback(int argc, char **argv, char **azColName)
{
	int i;
	std::map<std::string, std::string> map;
	std::cout << "Sql::callback with argc: " << argc << std::endl;
	for(i=0; i<argc; i++)
	{
		map.insert(std::pair<std::string, std::string>(std::string(azColName[i]), std::string(argv[i] ? argv[i] : "NULL")));
        std::cout << azColName[i] << "=" << std::string(argv[i] ? argv[i] : "NULL") << std::endl;
	}
	selectReturn.push_back(map);
    std::cout << std::endl;

	return 0;
}



std::vector<std::map<std::string, std::string> > Sql::executeQuery(std::string aQuery)
{
	char *zErrMsg = 0;

	int rc = sqlite3_exec(db, aQuery.c_str(), &Sql::callbackWrapper, (void *) this, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		std::cerr << "SQL error: " << zErrMsg << std::endl;
	
		sqlite3_free(zErrMsg);
	}
	std::string output("");
	auto returnValue = selectReturn;
    selectReturn.clear();

	return returnValue;
}

void Sql::addMeasurement(LibelIOPacket * packet)
{
    std::string sensorNames;
    std::string sensorDataStrings;
    std::map<SensorType, float> sensorData = packet->getSensorData();
    std::vector<unsigned char> zigbeeAddress = packet->getZigbee64BitAddress();
    auto sensorMap = Config::getSensorMap();
    for(auto it = sensorData.begin(); it != sensorData.end(); ++it)
    {

        auto foundSensorType = sensorMap.find(it->first);
        if(foundSensorType != sensorMap.end())
        {
            sensorNames.append("," + foundSensorType->second);
            sensorDataStrings.append(", '" + std::to_string(it->second) + "'");
        }
        else
        {
            std::cerr << "SEVERE CODE ERROR: SensorType was not found in sensormap" << std::endl;
        }

    }

    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time(); //use the clock

    std::stringstream stream;
    for(auto it = zigbeeAddress.begin(); it < zigbeeAddress.end(); ++it)
    {
        stream << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it);
    }


    std::string query("INSERT INTO sensordata (timestamp, zigbeeaddress" + sensorNames + ")" );
    query.append("VALUES('" + boost::posix_time::to_simple_string(now) + "', '" + stream.str() + "' " + sensorDataStrings + ")");
#ifdef SQL_DEBUG
    std::cout << "Add sensor data query: " << std::endl << query << std::endl << std::endl;
#endif
    executeQuery(query);

}

void Sql::addIpsumPacket(const std::string& url, const std::string& XML)
{
	std::string query("INSERT INTO ipsum_packets (url, XML) VALUES('");
	query.append(url);
	query.append("', '");
	query.append(XML);
	query.append("')");
	executeQuery(query);
}	

std::vector<std::map<std::string, std::string> > Sql::retrieveIpsumPacket()
{
	std::string query("SELECT *  FROM ipsum_packets");
	std::vector<std::map<std::string, std::string>> ipsumPacket = executeQuery(query);
	return ipsumPacket;
}

void  Sql::removeIpsumPacket(int id)
{
	std::string query("DELETE FROM ipsum_packets WHERE id = '");
	query.append(std::to_string(id));
	query.append("'");
	executeQuery(query);
}

bool Sql::makeNewNode(int installationID, int nodeID, std::string zigbee64BitAddress)
{
    std::cout << "makeNewNode begin" << std::endl;

    std::string query;
    bool exists = getNodeID(zigbee64BitAddress) != -1;
    if(exists)      // Check if zigbee64BitAddress already exists in DB. If so, this node has already been added.
    {
        deleteNode(zigbee64BitAddress);
        //query = "UPDATE Nodes SET installationID='" + std::to_string(installationID) + "', nodeID ='" + std::to_string(nodeID) + "' WHERE zigbee64bitaddress='" + zigbee64BitAddress + "'";

    }


    query = "INSERT INTO nodes (installationID, nodeID, zigbee64bitaddress) VALUES(";
    query.append(std::to_string(installationID) + ", " + std::to_string(nodeID) + ", '" + zigbee64BitAddress);
    query.append("')");


    std::cout << "query : " << query << std::endl;
	executeQuery(query);

    /*
	query.clear();
	query.append("SELECT * FROM nodes");
	auto vector = executeQuery(query);
	for(auto it = vector.begin(); it < vector.end(); ++it)
	{
		auto field = it->find("nodeID");
		if(field != it->end())
		{
			std::cout << "fieldname: " << field->first << "fieldvalue: " << field->second << std::endl;
		}
		field = it->find("zigbee64bitaddress");
		if(field != it->end())
		{
			std::cout << "fieldname: " << field->first << "fieldvalue: " << field->second << std::endl;
		}
	}
    */
    return !exists;

}

bool Sql::deleteNode(std::string zigbee64BitAddress)
{

    if(getNodeID(zigbee64BitAddress) == -1)      // Check if zigbee64BitAddress already exists in DB. If so, this node has already been added.
        return false;

    std::string query("DELETE FROM nodes WHERE zigbee64bitaddress = '" + zigbee64BitAddress + "'");
    executeQuery(query);

    return true;
}

/*void Sql::keepSensors(std::vector<SensorType>, int nodeID)
{
    sensorMap
    std::string query("DELETE FROM nodes WHERE zigbee64bitaddress = '" + zigbee64BitAddress + "'");
    executeQuery(query);
}*/

	
std::string Sql::updateSensorsInNode(int nodeID, SensorType sensorType, int sensorID)
{

    auto sensorMap = Config::getSensorMap();
    auto foundSensorType = sensorMap.find(sensorType);
    if(foundSensorType != sensorMap.end())
    {
        std::string query("UPDATE nodes SET " + foundSensorType->second + "ID =" + std::to_string(sensorID) + " WHERE nodeID=" + std::to_string(nodeID));
        executeQuery(query);
        return query;
    }
    else
    {
        std::cerr << "SEVERE CODE ERROR: SensorType was not found in sensormap" << std::endl;
        return "Error in Sql::updateSensorsInNode";
    }

}

std::string Sql::getNodeAddress(int nodeID) throw (SqlError)
{
    std::string query("SELECT zigbee64bitaddress FROM nodes WHERE nodeID = '" + std::to_string(nodeID) + "'");
	auto data = executeQuery(query);
    if(data.size() != 1)
    {
        std::cerr << "node address not found in DB" << std::endl;
        return "";
    }
	for(auto it = data.begin(); it < data.end(); ++it)
	{
		auto field = it->find("zigbee64bitaddress");
		if(field != it->end())
		{
			return field->second;		
		}
		
	}
    return std::string();
}

int Sql::getNodeID(std::string zigbeeAddress64Bit) throw (SqlError)
{
    std::string query("SELECT nodeID from nodes WHERE zigbee64bitaddress = '" + zigbeeAddress64Bit + "'");
	auto data = executeQuery(query);
    if(data.size() == 0)
    {
        return -1;
    }
    else if(data.size() != 1)
    {
        throw SqlError();   // To many entries with the same address
    }

	for(auto it = data.begin(); it < data.end(); ++it)
	{
		auto field = it->find("nodeID");
		if(field != it->end())
		{
			return boost::lexical_cast<int>(field->second);		
		}
	}
	return -1;
}

int Sql::getInstallationID(std::string zigbeeAddress64Bit) throw (SqlError)
{
    std::string query("SELECT installationID from nodes WHERE zigbee64bitaddress = '" + zigbeeAddress64Bit + "'");
    std::cout << "query in getInstallationID: " << query << std::endl;
	auto data = executeQuery(query);
    if(data.size() < 1)
    {
        throw SqlError();
    }
	for(auto it = data.begin(); it < data.end(); ++it)
	{
		auto field = it->find("installationID");
		if(field != it->end())
		{
			return boost::lexical_cast<int>(field->second);		
		}
		
	}
	return -1;
}

#define CHECKSENSOR(name, sensortype)\
    field = it->find(#name);\
	if(field != it->end())\
	{\
		if(field->second != std::string("-1"))\
		{\
			bool badCast = false;\
			int sensorID;\
			try\
			{\
				sensorID = boost::lexical_cast<int>(field->second);\
			}\
			catch(boost::bad_lexical_cast)\
			{\
				badCast = true;\
				std::cerr << "Nodes table contained a non number as SensorID, this sensor has been ignored" << std::endl;\
			}\
			if(!badCast)\
			{\
				sensors.insert(std::pair<SensorType,int>(sensortype, sensorID));\
			}\
		}\
	}

std::map<SensorType, int> Sql::getSensorsFromNode(int nodeID) throw (SqlError)
{
    std::string query("SELECT temperatureID, humidityID, pressureID, batteryID, co2ID, anemoID, vaneID, pluvioID, luminosityID, solar_radID  from  nodes WHERE nodeID = " + std::to_string(nodeID));
	auto data = executeQuery(query);
	std::cout << "data.size() " << data.size() << std::endl;
	if (data.size() != 1)
	{
		throw SqlError();
	}

	std::map<SensorType, int> sensors;
	auto it = data.begin();
	std::map<std::string, std::string>::iterator  field;
    auto sensorMap = Config::getSensorMap();
    for(auto sensorIt = sensorMap.begin(); sensorIt != sensorMap.end(); ++sensorIt)
    {
        field = it->find(sensorIt->second + "ID");
        if(field != it->end())
        {
            if(field->second != std::string("-1"))
            {
                bool badCast = false;
                int sensorID;
                try
                {
                    sensorID = boost::lexical_cast<int>(field->second);
                }
                catch(boost::bad_lexical_cast)
                {
                    badCast = true;
                    std::cerr << "Nodes table contained a non number as SensorID, this sensor has been ignored" << std::endl;
                }
                if(!badCast)
                {
                    sensors.insert(std::pair<SensorType,int>(sensorIt->first, sensorID));
                }
            }
        }
    }

    /*
	CHECKSENSOR(temperatureID, TEMP)
	CHECKSENSOR(humidityID, HUM)
	CHECKSENSOR(pressureID, PRES)
	CHECKSENSOR(batteryID, BAT)
	CHECKSENSOR(co2ID, CO2)
	CHECKSENSOR(anemoID, ANEMO)
	CHECKSENSOR(vaneID, VANE)
	CHECKSENSOR(pluvioID, PLUVIO)
    */
    /*
	field = it->find("temperatureID");
	if(field != it->end())
	{
		if(field->second != std::string("-1"))
		{	
			sensors.insert(std::pair<SensorType,int>(TEMP, boost::lexical_cast<int>(field->second)));
		}	
	}
	*/	

	return sensors;
}



