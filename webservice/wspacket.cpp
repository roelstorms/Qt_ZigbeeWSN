#include "wspacket.h"

WSPacket::WSPacket(std::string aURI, std::string aData) throw(WebserviceInvalidCommand) : Packet(), data(aData)
{
	std::cout << "WSPacket constructor" << std::endl;	
	if(aURI.find("changeFrequency") != std::string::npos)
	{
		std::cout << "request type set to CHANGE_FREQUENCY" << std::endl;
		requestType = CHANGE_FREQUENCY;
	}
	else if(aURI.find("addNode") != std::string::npos)
	{
		std::cout << "request type set to ADD_NODE" << std::endl;
		requestType = ADD_NODE;
	}
	else if(aURI.find("addSensor") != std::string::npos)
	{
		std::cout << "request type set to ADD_SENSOR" << std::endl;
		requestType = ADD_SENSOR;
	}
	else if(aURI.find("requestData") != std::string::npos)
	{
		std::cout << "request type set to ADD_SENSOR" << std::endl;
		requestType = ADD_SENSOR;
	}
	else
	{
		throw WebserviceInvalidCommand();
	}
	
}

WSPacket::~WSPacket()
{

}

const RequestType&  WSPacket::getRequestType() const
{
	return requestType;
}

const std::string& WSPacket::getRequestData() const
{
	return data;
}
