#ifndef ATCOMMANDRESPONSEPACKET_H
#define ATCOMMANDRESPONSEPACKET_H

#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include "packet.h"

class ATCommandResponsePacket : public Packet
{
	private:
	unsigned char frameID, commandStatus;
	std::vector <unsigned char> ATCommand, data;
	public:
	ATCommandResponsePacket();
	ATCommandResponsePacket(const Packet& aPacket);
	ATCommandResponsePacket(unsigned char aChecksum, unsigned char aType, unsigned char aSizeLSB, unsigned char aSizeMSB,std::vector<unsigned char> aEncodedPacket, unsigned char aFrameID, unsigned char aCommandStatus, std::vector <unsigned char>aATCommand, std::vector<unsigned char> aData);
	~ATCommandResponsePacket();
		
	void setFrameID(unsigned char aFrameID);
	void setCommandStatus(unsigned char aCommandStatus);
	void setATCommand(std::vector<unsigned char> aATCommand);
	void setData(std::vector<unsigned char> adata);
};

#endif
