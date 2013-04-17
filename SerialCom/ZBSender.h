#ifndef ZBSENDER_H
#define ZBSENDER_H

#include <boost/thread/mutex.hpp>
#include <queue>
#include "../packetqueue.h"
#include <mutex>
#include <condition_variable>
#include "packets/libelchangefreqpacket.h"
#include "packets/libelchangenodefreqpacket.h"
#include "packets/libeladdnodepacket.h"
#include "packets/libelrequestiopacket.h"
#include "packets/libelmaskrequest.h"
#include <typeinfo>

class ZBSender
{
	private:
    int connectionDescriptor;
    std::mutex  * zbSenderConditionVariableMutex;
	std::condition_variable * zbSenderConditionVariable;
	PacketQueue * zbSendQueue;	


	public:
    ZBSender(int connectionDescriptor, std::mutex * zbSenderConditionVariableMutex, std::condition_variable * zbSenderConditionVariable, PacketQueue * zbSendQueue);
    std::vector<unsigned char> escape(std::vector<unsigned char> data);
	void operator () ();


};

#endif
