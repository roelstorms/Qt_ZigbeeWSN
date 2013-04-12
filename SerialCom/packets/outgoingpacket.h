#ifndef OUTGOINGPACKET_H
#define OUTGOINGPACKET_H
#include "ZBPacket.h"

class OutgoingPacket : public ZBPacket
{
	private:

	public:
		OutgoingPacket();
		OutgoingPacket(std::vector<unsigned char> data );

};

#endif
