#ifndef OUTGOINGPACKET_H
#define OUTGOINGPACKET_H
#include "ZBPacket.h"

class OutgoingPacket : public ZBPacket
{
	private:
        int numberOfResends;
        int timeOfLastSending;  // If 0, then it hasn't been sent yet. ZBSender should alter this field.
	public:
		OutgoingPacket();
		OutgoingPacket(std::vector<unsigned char> data );

        int getNumberOfResends() const;
        void incrementNumberOfResends();

        int getTimeOfLastSending();
        void setTimeOfLastSending(int timeOfLastSending);
};

#endif
