#include "wspacket.h"

WSPacket::WSPacket(std::string URI, std::string data) throw(WebserviceInvalidCommand) : Packet(), data(data)
{
	std::cout << "WSPacket constructor" << std::endl;	
    if(URI.find("changeFrequency") != std::string::npos)
	{
		std::cout << "request type set to CHANGE_FREQUENCY" << std::endl;
		requestType = CHANGE_FREQUENCY;
	}
    else if(URI.find("addNode") != std::string::npos)
	{
		std::cout << "request type set to ADD_NODE" << std::endl;
		requestType = ADD_NODE;
	}
    else if(URI.find("addSensor") != std::string::npos)
	{
		std::cout << "request type set to ADD_SENSOR" << std::endl;
		requestType = ADD_SENSOR;
	}
    else if(URI.find("requestData") != std::string::npos)
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
