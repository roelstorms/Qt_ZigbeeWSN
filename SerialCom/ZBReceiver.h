#ifndef ZBRECEIVER_H
#define ZBRECEIVER_H

#include <string>
#include <boost/thread.hpp>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include <fstream>

#include "../HTTP/http.h"
#include "../XML/XML.h"
#include "../packetqueue.h"

#include "packets/libeliopacket.h"
#include "packets/libeladdnoderesponse.h"
#include "packets/libelchangefreqresponse.h"
#include "packets/libelchangenodefreqresponse.h"
#include "packets/libelmaskresponse.h"

#include "../errors.h"
#include <boost/thread/mutex.hpp>
#include <queue>
#include <mutex>
#include <condition_variable>


class ZBReceiver
{
	private:
	std::ofstream logFile;
	int fileDescriptor;
	std::mutex  * conditionVariableMutex;
	std::condition_variable * mainConditionVariable;
	PacketQueue * zbReceiveQueue;	
	// Copy constructor
	ZBReceiver(const ZBReceiver& source);	 

	public:
	ZBReceiver(int fd, std::mutex * aConditionVariableMutex, std::condition_variable * aMainConditionVariable, PacketQueue * aZBReceiveQueue);
	~ZBReceiver();
	


	unsigned char readByte(int fd);
	void operator () ();
	

};

#endif
