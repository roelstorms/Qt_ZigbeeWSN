#include "dataIOPacket.h"


/*
DataIOPacket::DataIOPacket(const Packet& aPacket) : Packet(aPacket)
{
	std::cout << "DataIO constructor(Packet)" << std::endl;
}
*/

DataIOPacket::DataIOPacket(std::vector<unsigned char> input) : IncomingPacket(input)
{
	std::cout << std::endl << "DataIOPacket(std::vector<unsigned char> input)" << std::endl;
	for(auto it = input.begin(); it < input.end(); ++it)
	{
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int) (*it) << std::endl;
	}
	if(input.at(3) != 0x92)	std::cout << "Packet type identifier doesn't match with a data io packet" << std::endl;
	sourceAddress = std::vector<unsigned char>(input.begin() + 4, input.begin() + 12);	
	std::cout << "sourceAddress size: " << sourceAddress.size() << std::endl;
networkAddress = std::vector<unsigned char>(input.begin() + 12, input.begin() + 14);
	receiveOptions = input.at(14);
	numberOfSamples = input.at(15);


	unsigned char digitalChannelMaskChar =  input.at(17);
	for(int i = 0; i < 8; ++i)
	{	
		digitalChannelMask.push_back(digitalChannelMaskChar & 0x01);
		digitalChannelMaskChar = digitalChannelMaskChar >> 1;
	}
	digitalChannelMaskChar =  input.at(16);
	for(int i = 0; i < 8; ++i)
	{	
		digitalChannelMask.push_back(digitalChannelMaskChar & 0x01);
		digitalChannelMaskChar = digitalChannelMaskChar >> 1;
	}

	std::cout << "digitalChannelMask: " << std::endl;
	for(auto it = digitalChannelMask.begin(); it < digitalChannelMask.end(); ++it)
	{
		
		std::cout << std::uppercase << std::hex << (int) (*it) << " ";
	}
	std::cout << std::endl << std::endl;
	
	unsigned char analogChannelMaskChar =  input.at(18);
	for(int i = 0; i < 8; ++i)
	{	
		analogChannelMask.push_back(analogChannelMaskChar & 0x01);
		analogChannelMaskChar = analogChannelMaskChar >> 1;
	}
	
	
	std::cout << "analogChannelMask: " << std::endl;
	for(auto it = analogChannelMask.begin(); it < analogChannelMask.end(); ++it)
	{
		
		std::cout << std::uppercase << std::hex << (int) (*it) << " ";
	}
	std::cout << std::endl << std::endl;



	std::cout << "sourceAddress: " << std::endl;
	for(auto it = sourceAddress.begin(); it < sourceAddress.end(); ++it)
	{
		
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int) (*it);
	}
	std::cout << std::endl << std::endl;

	
	std::cout << "networkAddress: " << std::endl;
	for(auto it = networkAddress.begin(); it < networkAddress.end(); ++it)
	{
		
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int) (*it);
	}
	std::cout << std::endl << std::endl;
	
	std::cout << "digitalChannelMask: " << std::endl;
	for(auto it = digitalChannelMask.begin(); it < digitalChannelMask.end(); ++it)
	{
		
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int) (*it);
	}
	std::cout << std::endl << std::endl;


	int count = 19;
	
	for(auto it = digitalChannelMask.begin(); it < digitalChannelMask.end(); ++it)
	{
		if((*it) == 1)
		{
			digitalInput.push_back(input.at(count));
			count = 21;
		}
		else
		{
			digitalInput.push_back(0);
		}
	}

	for(auto it = analogChannelMask.begin(); it < analogChannelMask.end(); ++it)
	{
		if((*it) == 1 )
		{
			analogInput.push_back(input.at(count));
			count++;
			analogInput.push_back(input.at(count));
			count++;
		}
		else
		{
			analogInput.push_back(0);
			analogInput.push_back(0);
		}
	}
	
	std::cout << std::endl << "analogInput" << std::endl;
	for(auto it = analogInput.begin(); it < analogInput.end(); ++it)
	{
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int) (*it);
	}
	
	std::cout << std::endl << "digitalInput" << std::endl;
	for(auto it = digitalInput.begin(); it < digitalInput.end(); ++it)
	{
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int) (*it);
	}
	std::cout << std::endl;
	//std::cout << "analogChannelMask: " << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int) analogChannelMask <<  std::endl << std::endl;
	//std::cout << "receiveOptions: " << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int) receiveOptions <<  std::endl << std::endl;
	//std::cout << "nrOfSamples: " << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int) numberOfSamples <<  std::endl << std::endl;

	

	std::cout << "DataIO constructor" << std::endl;
}

DataIOPacket::~DataIOPacket()
{
	std::cout << "DataIOPacket destructor" << std::endl;
}



double DataIOPacket::readAnalog(int pin)
{
	if(analogChannelMask.at(pin) != 0)
	{
		return ((256 * (double)analogInput.at(pin * 2) + (double)analogInput.at(pin * 2 + 1)) * 1.2 / 0x3FF);
	}
	std::cout << "You tried to read an input which was not enabled" << std::endl;
	return -1;
}

bool DataIOPacket::readDigital(int pin)
{
	if(digitalChannelMask.at(pin) != 0)
	{
		return digitalInput.at(pin);
	}
	std::cout << "You tried to read an input which was not enabled" << std::endl;
	return false;
}	

void DataIOPacket::setSourceAddress(std::vector<unsigned char> aSourceAddress)
{
	
}

void DataIOPacket::setNetworkAddress(std::vector<unsigned char> aNetworkAddress)
{

}

void DataIOPacket::setReceiveOptions(unsigned char)
{

}

void DataIOPacket::setNumberOfSamples(unsigned char)
{

}

void DataIOPacket::setDigitalChannelMask(std::vector<unsigned char>)
{

}

void DataIOPacket::setAnalogChannelMask(std::vector<unsigned char>)
{

}
