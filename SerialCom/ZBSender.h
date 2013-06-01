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
#include <fstream>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

class ZBSender
{
	private:
    std::ofstream logFile;
    int connectionDescriptor;
    std::mutex  * zbSenderConditionVariableMutex;
	std::condition_variable * zbSenderConditionVariable;
	PacketQueue * zbSendQueue;	
    bool * stop;

	public:
    ZBSender(bool * stop, int connectionDescriptor, std::mutex * zbSenderConditionVariableMutex, std::condition_variable * zbSenderConditionVariable, PacketQueue * zbSendQueue);
    std::vector<unsigned char> escape(std::vector<unsigned char> data);
	void operator () ();


};

#endif
