/*
 *  Created by Roel Storms
 *
 *  A thread is created of the ZBSender() function. This thread sends all packets
 *  that should go into the ZigBee network. Escaping of data is also done here.
 *
 *  All incoming packets can also be logged to a file accompanied by a timestamp.
 *
 */

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
#include "enums.h"

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
