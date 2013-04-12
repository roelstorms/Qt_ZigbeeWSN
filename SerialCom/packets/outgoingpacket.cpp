#include "outgoingpacket.h"

OutgoingPacket::OutgoingPacket() : ZBPacket()
{

}

OutgoingPacket::OutgoingPacket(std::vector<unsigned char> data) : ZBPacket ()
{
	setFrameData(data);	
}


