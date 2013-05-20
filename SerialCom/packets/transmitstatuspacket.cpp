#include "transmitstatuspacket.h"

TransmitStatusPacket::TransmitStatusPacket(std::vector<unsigned char> input) throw (InvalidPacketType): IncomingPacket(input)
{
    std::cout << "TransmitStatusPacket(std::vector<unsigned char> input)" << std::endl;
    if(getFrameType() != 0x8B)
    {
        throw InvalidPacketType();
    }
    std::cout << "end of TransmitStatusPacket constructor" << std::endl;
}


unsigned char TransmitStatusPacket::getFrameID() const
{
    std::vector<unsigned char> frameData = getFrameData();
    return frameData.at(1);
}

unsigned char TransmitStatusPacket::getDeliveryStatus() const
{
    std::vector<unsigned char> frameData = getFrameData();
    return frameData.at(5);
}


unsigned char TransmitStatusPacket::getDiscoveryStatus() const
{
    std::vector<unsigned char> frameData = getFrameData();
    return frameData.at(6);
}
