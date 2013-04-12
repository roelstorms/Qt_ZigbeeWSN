#include "XML.h"
#include <utility>

int main(int argc, char* argv[])
{
	XML XMLParser;


	std::vector<std::pair<std::string, std::string>> aListOfFields;
	aListOfFields.push_back(std::pair<std::string, std::string>(std::string("namefield2"), std::string("decimal_ps_")));
	aListOfFields.push_back(std::pair<std::string, std::string>(std::string("namefield1"), std::string("float_n_")));
	std::cout << XMLParser.createNewType(std::string("aName"), aListOfFields) << std::endl;		
	
	std::cout << std::endl << std::endl;
	XMLParser.uploadData(std::string("type"), std::string("value"), 22.5, std::string("11-12-31T00:01:22"));

	return 0;	
}

