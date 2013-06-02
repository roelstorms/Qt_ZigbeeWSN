/*
 *	Created by Roel Storms
 *
 *  The function Ipsum() is used to create a thread in the main thread. This Ipsum thread an incoming and outgoing packet queue between main and itself.
 *  Incoming packets are processed by the corresponding Handler functions. The outgoing packet queue is not used yet but can be used to send packets back to main.
 *  For instance when the Ipsum remote data storage is not reachable, packets can be sent back to main to store into a database untill ipsum can be reached again.
 *
 *  If Ipsum is unreachable now, packets are cached on a local queue in this thread. But no measures have been taken if Ipsum is down for a long period of time.
 */


#ifndef IPSUM_H
#define IPSUM_H
#include <mutex>
#include <condition_variable>
#include "../packet.h"
#include "../packetqueue.h"
#include <string>
#include <iostream>
#include <queue>
#include "ipsumuploadpacket.h"
#include "http.h"

class Ipsum
{
	private:
	PacketQueue * ipsumSendQueue, * ipsumReceiveQueue;
	std::mutex * mainConditionVariableMutex, * ipsumConditionVariableMutex;
	std::condition_variable * mainConditionVariable, * ipsumConditionVariable;
	std::queue<Packet *> * localIpsumSendQueue;

	Http * http;

    bool IpsumUnreachable, * stop;

	Ipsum(const Ipsum&);
	public:
    Ipsum(bool * stop, std::string ipsumURL, std::string ipsumPersonalKey, PacketQueue * ipsumSendQueue, PacketQueue * ipsumReceiveQueue, std::mutex * mainConditionVariableMutex, std::condition_variable * mainConditionVariable, std::mutex * ipsumConditionVariableMutex, std::condition_variable * ipsumConditionVariable);
	~Ipsum();
	void operator() ();


	void uploadDataHandler(IpsumUploadPacket * packet);
	void changeInUseHandler(IpsumChangeInUsePacket * packet);
	void changeFrequencyHandler(IpsumChangeFreqPacket * packet);

};

#endif

