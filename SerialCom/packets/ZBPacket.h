#ifndef ZBPACKET_H
#define ZBPACKET_H

#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include <vector>
#include <ostream>
#include <iomanip>
#include "../../packet.h"

class ZBPacket : public Packet
{
	private:

	//Packet(const Packet& aPacket){}
	protected:
	bool validPacket;
	ZBPacket();
	std::vector<unsigned char> encodedPacket;
	public:
	//ZBPacket(std::vector<unsigned char> input);
	~ZBPacket();
	PacketType getPacketType() = 0;

	unsigned char getChecksum() const;
	unsigned char getSizeLSB() const;
	unsigned char getSizeMSB() const;
	const std::vector<unsigned char>& getEncodedPacket() const;
	bool getValidPacket() const;

	std::vector<unsigned char> getFrameData() const throw (ZbCorruptedFrameData);
	void setFrameData(std::vector<unsigned char> frameData);
	
	unsigned char getFrameType() const;
	
	friend std::ostream &operator<<(std::ostream &out, ZBPacket &packet)     //output
	{
		
		std::vector<unsigned char> encodedPacket = packet.getEncodedPacket();
		for(auto it = encodedPacket.begin(); it < encodedPacket.end(); ++it)
		{
			std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it) << " ";
	
		}

		return out;
	}
};

#endif
