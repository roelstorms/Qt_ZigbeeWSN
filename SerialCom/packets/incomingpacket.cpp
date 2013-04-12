#include "incomingpacket.h"

IncomingPacket::IncomingPacket(std::vector<unsigned char> input) throw (CorruptedPacket) : ZBPacket()
{
	std::cout << "IncomingPacket(std::vector<unsigned char> input)" << std::endl;
	if(input.front() != 0x7E)
	{
		std::cout << "error: invalid packet" << std::endl;
		throw CorruptedPacket();
	}
	encodedPacket = input;

	input.erase(input.begin());
	unsigned int sizeMSB = input.front();
	input.erase(input.begin());

	int sizeLSB =  input.front();
	input.erase(input.begin());
	
	unsigned int size = (256 * sizeMSB) + sizeLSB;
	
	unsigned char sum = 0;
	for(unsigned int i = 0; i < input.size(); ++i)
	{
		sum += input.at(i);
	}
	std::cout << "sum: " << std::hex <<  (int)sum << std::endl;

	if((input.size() - 1) != size)			// -1 because the checksum is not counted
	{
		std::cout << "input.size: " << input.size() - 1	<< std::endl << "size: " << size << std::endl;
		std::cout << "size does not match" << std::endl;
		throw CorruptedPacket();
	}
	else if(sum != 0xFF)
	{
		std::cerr << "checksum is wrong, corrupted packed" << std::endl;
		throw CorruptedPacket();
	}
	else
	{
		validPacket = true;
	}

}
