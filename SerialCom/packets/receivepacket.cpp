#include "receivepacket.h"

ReceivePacket::ReceivePacket(std::vector<unsigned char> input) : IncomingPacket(input)
{
	std::cout << "ReceivePacket(std::vector<unsigned char> input)" << std::endl;
	if(getFrameType() != 0x90)	std::cerr << "Packet type identifier doesn't match with a data packet" << std::endl;
	unescapeData();
	std::cout << "end of DataPacket constructor" << std::endl;
}


void ReceivePacket::unescapeData()
{
	std::cout << "unescaping data in a libelium packet" << std::endl;
	std::vector<unsigned char> unescapedData;
	auto rfData = getRFData();
	for(auto it = rfData.begin(); it < rfData.end(); ++it)
	{
		if((*it) == 0xFF)
		{
			++it;
			switch (*it)
			{
				case 0xFE:
					unescapedData.push_back(0X00);
				break;
				case 0xFD:
					unescapedData.push_back(0xFF);
			}
		}
		else
		{
			unescapedData.push_back(*it);
		}
	}
	std::cout << "unescaped data: " << std::endl;

	for(auto it = unescapedData.begin(); it < unescapedData.end(); ++it)
	{
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it);
	}
	std::cout << std::endl;
	setRFData(unescapedData);

}

void ReceivePacket::setRFData(std::vector<unsigned char> rfData)
{
	auto frameData = getFrameData();
	std::cout << "before setRF" << std::endl;
	for(auto it = frameData.begin(); it < frameData.end(); ++it)
	{
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it);
	}
	std::cout << std::endl;
	
	frameData.erase(frameData.begin() + 12, frameData.end());
	std::cout << "after erase setRF" << std::endl;
	for(auto it = frameData.begin(); it < frameData.end(); ++it)
	{
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it);
	}
	std::cout << std::endl;
	
	frameData.insert(frameData.begin() + 12, rfData.begin(), rfData.end());
	for(auto it = frameData.begin(); it < frameData.end(); ++it)
	{
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it);
	}
	std::cout << std::endl << "after setRF" << std::endl;
	setFrameData(frameData);
}

// Bits in the mask have this meaning: 15,14,13,12,11,10,9,8,PLUVIO, ANEMO, CO2, BAT, PRES, HUMID, TEMP
std::vector<unsigned char> ReceivePacket::getMask() const
{
	/*
	   std::vector<bool> mask;

	unsigned int maskChars = getRFData().at(1) * 256 + getRFData().at(2);
	for(int i = 0; i < 16; ++i)
	{	
		mask.push_back(maskChars & 0x0001);
		maskChars = maskChars >> 1;
	}
	*/
	std::vector<unsigned char> mask;
	mask.push_back(getRFData().at(6));
	mask.push_back(getRFData().at(7));
	return mask;
}

std::vector<unsigned char> ReceivePacket::getZigbee16BitAddress() const
{
	auto frameData = getFrameData();
	std::vector<unsigned char> zigbee16BitAddress(frameData.begin() + 9, frameData.begin() + 11);

	return zigbee16BitAddress;	
}

std::vector<unsigned char> ReceivePacket::getZigbee64BitAddress() const
{
	auto frameData = getFrameData();
	std::vector<unsigned char> zigbee64BitAddress(frameData.begin() + 1, frameData.begin() + 9);

	return zigbee64BitAddress;	
}


std::vector<unsigned char> ReceivePacket::getRFData() const throw(ZbCorruptedFrameData)
{
	std::vector<unsigned char> frameData = getFrameData();
	if(frameData.begin() + 12 >= frameData.end())
	{
		std::cerr << "No RFData in this receivepacket packet" << std::endl;
		throw ZbCorruptedFrameData();
	}

	std::vector<unsigned char> output(frameData.begin() + 12, frameData.end());
	return output;
	
}

std::vector<unsigned char> ReceivePacket::getData() const throw(ZbCorruptedFrameData)
{
	std::vector<unsigned char> rfData = getRFData();
	return std::vector<unsigned char> (rfData.begin() + 5, rfData.end());
}
