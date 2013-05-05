#include "transmitrequestpacket.h"

TransmitRequestPacket::TransmitRequestPacket() : OutgoingPacket()
{

}


void TransmitRequestPacket::setData( unsigned char applicationID, std::vector<unsigned char> zigbeeAddress64Bit, std::vector<unsigned char> data)
{
    std::vector<unsigned char> applicationData;
    applicationData.push_back(applicationID);
    applicationData.push_back(0x01);                // Fragment number or in case of first fragment it means total number of fragments
    applicationData.push_back('#');                 // First fragment identifier
    applicationData.push_back(0x01);                // Source ID (1 means 64bit)    This is not the most effecient way, 16 bit address would be better but would have to look it up or save it as well. 16 bit address can change
    applicationData.insert(applicationData.end(), zigbeeAddress64Bit.begin(), zigbeeAddress64Bit.end());
    applicationData.insert(applicationData.end(), data.begin(), data.end());
    setRFData(zigbeeAddress64Bit, applicationData);
}

void TransmitRequestPacket::setRFData(std::vector<unsigned char> zigbeeAddress64Bit, std::vector<unsigned char> applicationData)
{
    std::vector<unsigned char> rfData;
    rfData.push_back(0x10);     // TransmitRequest has Frame Type 10
    rfData.push_back(0x00);     // Frame ID, if set to 0 no ack will be sent. Else an ack with this ID will be sent back.
    rfData.insert(rfData.end(), zigbeeAddress64Bit.begin(), zigbeeAddress64Bit.end());
    rfData.push_back(0xFF);     // Zigbee 16bit address, if unknown, set to FF FE
    rfData.push_back(0xFE);
    rfData.push_back(0x00);     // Broadcast radius
    rfData.push_back(0x00);     // Options
    rfData.insert(rfData.end(), applicationData.begin(), applicationData.end());
    setFrameData(rfData);
}

std::vector<unsigned char> TransmitRequestPacket::getMask() const
{
	std::vector<unsigned char> mask;
	mask.push_back(getRFData().at(1));
	mask.push_back(getRFData().at(2));
	return mask;
}

std::vector<unsigned char> TransmitRequestPacket::getZigbee16BitAddress() const
{
	auto frameData = getFrameData();
	std::vector<unsigned char> zigbee16BitAddress(getFrameData().begin() + 10, getFrameData().begin() + 12);

	return zigbee16BitAddress;
}

std::vector<unsigned char> TransmitRequestPacket::getZigbee64BitAddress() const 
{
	auto frameData = getFrameData();
	std::vector<unsigned char> zigbee64BitAddress(getFrameData().begin() + 2, getFrameData().begin() + 10);

	return zigbee64BitAddress;	
}

std::vector<unsigned char> TransmitRequestPacket::getRFData() const throw (ZbCorruptedFrameData)
{
	std::vector<unsigned char> frameData = getFrameData();
	if(frameData.begin() + 14 >= frameData.end())
	{
		std::cerr << "No RFData in this receivepacket packet" << std::endl;
		throw ZbCorruptedFrameData();
	}

	std::vector<unsigned char> output(frameData.begin() + 14, frameData.end());
	return output;
}

std::vector<unsigned char> TransmitRequestPacket::getData() const throw(ZbCorruptedFrameData)
{
    std::vector<unsigned char> rfData = getRFData();
    return std::vector<unsigned char> (rfData.begin() + 5, rfData.end());
}

