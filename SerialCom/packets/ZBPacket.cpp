#include "ZBPacket.h"

/*
Packet::Packet(const Packet& aPacket)
{
	std::cout << "Copyconstructor" << std::endl;
	checksum = aPacket.getChecksum();
	type = aPacket.getType();
	sizeLSB = aPacket.getSizeLSB();
	sizeMSB = aPacket.getSizeMSB();
	encodedPacket = aPacket.getEncodedPacket();
}
*/

/*
ZBPacket::ZBPacket(std::vector<unsigned char> input, PacketType aType) : Packet(aType), encodedPacket(input)
{
	if(input.front() != 0x7E)
	{
		std::cout << "error: invalid packet" << std::endl;
	}

	input.erase(input.begin());
	unsigned int sizeMSB = input.front();
	input.erase(input.begin());

	int sizeLSB =  input.front();
	input.erase(input.begin());
	
	unsigned int size = (256 * sizeMSB) + sizeLSB;

	if((input.size() - 1) != size)			// -1 because the checksum is not counted
	{
		std::cout << "input.size: " << input.size() << std::endl << "size: " << size << std::endl;
		std::cout << "size does not match" << std::endl;
	}	
	
	unsigned char sum = 0;
	for(unsigned int i = 0; i < input.size(); ++i)
	{
		sum += input.at(i);
	}
	std::cout << "sum: " << std::hex <<  (int)sum << std::endl;
	if(sum != 0xFF)
	{
		std::cerr << "checksum is wrong, corrupted packed" << std::endl;
	}
}

ZBPacket::ZBPacket( std::vector<unsigned char> data, PacketType packetType) : Packet(packetType)
{
	setFrameData(data, frameType);	
	std::cout << "Packet constructor" << std::endl;
}
*/

ZBPacket::ZBPacket() : Packet()
{

}

ZBPacket::~ZBPacket()
{
	std::cout << "Packet destructor" << std::endl;
}


unsigned char ZBPacket::getFrameType() const
{
	return encodedPacket.at(3);
}

unsigned char ZBPacket::getChecksum() const
{
	return encodedPacket.at(encodedPacket.size()-1);
}

unsigned char ZBPacket::getSizeLSB() const
{
	return encodedPacket.at(2);
}

unsigned char ZBPacket::getSizeMSB() const
{
	return encodedPacket.at(1);
}

const std::vector<unsigned char>& ZBPacket::getEncodedPacket() const
{
	return encodedPacket;
}

bool ZBPacket::getValidPacket() const
{
	return validPacket;
}

std::vector<unsigned char> ZBPacket::getFrameData() const throw (ZbCorruptedFrameData)
{
	if(encodedPacket.begin() + 3 >= encodedPacket.end() - 1)
	{
		throw ZbCorruptedFrameData();
	}
	std::vector<unsigned char> data = std::vector<unsigned char>(encodedPacket.begin() + 3, encodedPacket.end() - 1);
	return data;
}

void ZBPacket::setFrameData(std::vector<unsigned char> frameData)
{
	encodedPacket.clear();
	encodedPacket.push_back(0x7E);
	unsigned int size = frameData.size() ;
	unsigned int sizeMSB = size/256;
	unsigned int sizeLSB = size%256; //- (sizeMSB * 256);
	encodedPacket.push_back(sizeMSB);
	encodedPacket.push_back(sizeLSB);
	//std::cout << "size of packet MSB: " << std::hex << (int)sizeMSB << "LSB: " << std::hex << (int)sizeLSB << std::endl;
	int sum = 0;
	for(auto  it = frameData.begin(); it < frameData.end(); ++it)
	{
		//std::cout << std::hex << (int) (*it) << std::endl;
		encodedPacket.push_back(*it);
		sum += (*it);
	}
	int checksum = 0xFF - sum;
	encodedPacket.push_back(checksum);

}

