#include "outgoingpacket.h"

OutgoingPacket::OutgoingPacket() : ZBPacket(), numberOfResends(0)
{

}

OutgoingPacket::OutgoingPacket(std::vector<unsigned char> data) : ZBPacket (), numberOfResends(0)
{
    setFrameData(data);
}

int OutgoingPacket::getNumberOfResends() const
{
    return numberOfResends;
}

void OutgoingPacket::incrementNumberOfResends()
{
    numberOfResends++;
}

int OutgoingPacket::getTimeOfLastSending()
{
    return timeOfLastSending;
}

void OutgoingPacket::setTimeOfLastSending(int timeOfLastSending)
{
    this->timeOfLastSending = timeOfLastSending;
}


