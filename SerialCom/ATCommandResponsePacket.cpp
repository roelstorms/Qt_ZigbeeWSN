#include "ATCommandResponsePacket.h"

ATCommandResponsePacket::ATCommandResponsePacket()
{
	std::cout << "ATCommandResponse constructor()" << std::endl;
}

ATCommandResponsePacket::ATCommandResponsePacket(const Packet& aPacket) : Packet(aPacket)
{
	std::cout << "ATCommandResponse constructor(Packet)" << std::endl;
}

ATCommandResponsePacket::ATCommandResponsePacket(unsigned char aChecksum, unsigned char aType, unsigned char aSizeLSB, unsigned char aSizeMSB,std::vector<unsigned char> aEncodedPacket, unsigned char aFrameID, unsigned char aCommandStatus, std::vector <unsigned char>aATCommand, std::vector<unsigned char> aData) : Packet(aChecksum, aType, aSizeLSB, aSizeMSB, aEncodedPacket), frameID(aFrameID), commandStatus(aCommandStatus), ATCommand(aATCommand), data(aData) 
{
	std::cout << "ATCommandResponse constructor" << std::endl;
}

ATCommandResponsePacket::~ATCommandResponsePacket()
{
	std::cout << "ATCommandResponse destructor" << std::endl;
}

void ATCommandResponsePacket::setFrameID(unsigned char aFrameID)
{
	frameID = aFrameID;
}

void ATCommandResponsePacket::setCommandStatus(unsigned char aCommandStatus)
{
	commandStatus = aCommandStatus;
}

void ATCommandResponsePacket::setATCommand(std::vector<unsigned char> aATCommand)
{
	ATCommand = aATCommand;
}

void ATCommandResponsePacket::setData(std::vector<unsigned char> aData)
{
	data = aData;
}
