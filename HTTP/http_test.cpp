#include "http.h"
#include "../XML/XML.h"

#include <string>

int main(int argc, char * argv[])
{
	//Http socket(std::string("localhost:8080"));//http://ipsum.groept.be"));
	Http socket(std::string("http://ipsum.groept.be"));

	std::cout << "adding sensor" << socket.createNewSensor("320", "getchildren test", "0", "0", "false") << std::endl;
	std::cout << socket.getChildren(socket.calculateDestination(21, 31,320)) << std::endl ;

	/*
	std::vector<std::pair<std::string, std::string>> aListOfFields;
	aListOfFields.push_back(std::pair<std::string, std::string>(std::string("temperature"), std::string("float_n_")));
	aListOfFields.push_back(std::pair<std::string, std::string>(std::string("humidity"), std::string("float_n_")));
	aListOfFields.push_back(std::pair<std::string, std::string>(std::string("pressure"), std::string("float_n_")));
	aListOfFields.push_back(std::pair<std::string, std::string>(std::string("CO2"), std::string("float_n_")));
	aListOfFields.push_back(std::pair<std::string, std::string>(std::string("battery"), std::string("int_")));

	socket.createNewType(std::string("libeliumSensorType"), aListOfFields);
	*/

	/*
	socket.createNewSensor(std::string("320"), std::string("Libelium01"), std::string("dede9bb15bdb8490997179d013fd41346"), std::string("First libelium sensor board that sends all data"), std::string("true"));
	*/


	/*
	   socket.setUserRights("<?xml version=\"1.0\" encoding=\"utf-16\"?><Sensor><location>14.04</location><sensorgroupid>320</sensorgroupid><frequency>5</frequency><dataname>dede9bb15bdb8490997179d013fd41346</dataname><id>2421</id><name>Libelium01</name><description>First libelium sensor board that sends all data</description><inuse>True</inuse><infoname></infoname></Sensor>", 21, 255);
	*/

	/*	
	std::vector<std::pair<std::string, double>> input;
	input.push_back(std::pair<std::string, double> (std::string("temperature"), 1.2));
	input.push_back(std::pair<std::string, double> (std::string("humidity"), 2.3));
	input.push_back(std::pair<std::string, double> (std::string("pressure"), 3.4));
	input.push_back(std::pair<std::string, double> (std::string("co2"), 4.5));
	input.push_back(std::pair<std::string, double> (std::string("battery"), 5.6));
	socket.uploadData(std::string("libeliumSensorType"), socket.calculateDestination(21, 31, 320, 2421), input); 
	*/
	 
	/*	
	std::vector<std::string> fieldNames;
	fieldNames.push_back(std::string("temperature"));
	fieldNames.push_back(std::string("humidity"));
	fieldNames.push_back(std::string("pressure"));
	fieldNames.push_back(std::string("co2"));
	fieldNames.push_back(std::string("battery"));
	std::cout << "data from ipsum: " << std::endl << socket.selectData(socket.calculateDestination(21, 31, 320, 2421), fieldNames) << std::endl;
	*/

	/*
	std::cout << "reply from own webservice" << socket.sendPost(std::string("/lol"), std::string("POST DATA TEST LONG TEKST\r") ,  &Http::standardReplyWrapper) << std::endl;	
	*/


	//std::cout << "entity: " <<  socket.getEntity(socket.calculateDestination(21, 31, 320, 2421)) << std::endl ; 	


	//socket.login();
//	socket.getEntity(socket.calculateDestination(21, 31, 320, 2041));
//socket.setUserRights("<?xml version=\"1.0\" encoding=\"utf-16\"?><Sensor><location></location><sensorgroupid>320</sensorgroupid><frequency>5</frequency><dataname>d3bae7e83c3fd4165b90570163927da19</dataname><id>2041</id><name>Module4LightSens</name><description>a test sensor to see if this request works</description><inuse>True</inuse><infoname></infoname></Sensor>",21, 255);
	
	

	//socket.testQuery();	
//	socket.uploadData(0.8);
	//socket.uploadData(2.1);
	/*	
		XML XMLParser;
		std::string url("/auth/");
		std::string temp;
		temp.append(url);
		url.append(socket.generateCode(temp.append("a31dd4f1-9169-4475-b316-764e1e737653")));

		socket.sendPost(url, XMLParser.login(std::string("roel"), std::string("roel")), &Http::loginReplyWrapper);
		std::string token = XMLParser.analyzeLoginReply("./log.txt");	
		url.clear();
	//url.append("/addInstall/");
	//url.append("/addGroup/");
	url.append("/addSensor/");
	//url.append("/addType/");
	//url.append("/upload");
	//url.append("/query/");
	url.append(token);
	url.append("/");
	//url.append(socket.calculateDestination(21, 31, 320, 2029));
	//url.append("/");
*/

/*
	temp.clear();
	temp.append(url);
	temp.append("a31dd4f1-9169-4475-b316-764e1e737653");
	url.append(socket.generateCode(temp));

	//socket.sendPost(url, XMLParser.createNewInstallation(std::string("GroepT"), std::string("a test installation to see if this request works"), std::string ("True")));

	//socket.sendPost(url, XMLParser.createNewSensorGroup(std::string("31"), std::string("Module 14"), std::string("a test sensor groep to see if this request works"), std::string ("True")));

	socket.sendPost(url, XMLParser.createNewSensor(std::string("320"), std::string("Module4LightSens"), std::string("d3bae7e83c3fd4165b90570163927da19"), std::string("a test sensor to see if this request works"), std::string ("True")), &Http::standardReplyWrapper);

	//socket.sendPost(url, XMLParser.createNewType(std::string("lightSensor"), std::string("intensity")));

	//std::pair<std::string, double> pair;
	//pair.first = std::string("field");
	//pair.second = 1.1;
	//std::vector<std::pair<std::string, double>> input;
	//input.push_back(pair);
	//pair.first = std::string("type2");
	//pair.second = 0.6;
	//input.push_back(pair);


	//socket.sendPost(url, XMLParser.uploadData(std::string("lightSensorRoel"), input));


	//std::string xml("<?xml version=\"1.0\" encoding=\"utf-8\"?><get><start>2012-01-01T00:00:00</start><end>9999-12-31T23:59:59</end><select><field><name>temperature</name></field></select></get>");*/
	//socket.sendPost(url, xml);
	return 0;
}
