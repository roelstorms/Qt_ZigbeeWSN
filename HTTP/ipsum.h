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

    bool IpsumUnreachable;

	Ipsum(const Ipsum&);
	public:
	Ipsum(PacketQueue * ipsumSendQueue, PacketQueue * ipsumReceiveQueue, std::mutex * mainConditionVariableMutex, std::condition_variable * mainConditionVariable, std::mutex * ipsumConditionVariableMutex, std::condition_variable * ipsumConditionVariable);
	~Ipsum();
	void operator() ();


	void uploadDataHandler(IpsumUploadPacket * packet);
	void changeInUseHandler(IpsumChangeInUsePacket * packet);
	void changeFrequencyHandler(IpsumChangeFreqPacket * packet);

};

#endif

