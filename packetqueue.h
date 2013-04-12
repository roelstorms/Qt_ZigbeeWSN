#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#include <mutex>
#include "packet.h"
#include <queue>
#include <condition_variable>
#include "SerialCom/packets/dataIOPacket.h"
#include "packet.h"
#include "errors.h"

class PacketQueue
{
	private:
		std::recursive_mutex mutex;
		std::queue<Packet *> queue;	
	
		PacketQueue(PacketQueue&);
		PacketQueue(const PacketQueue&);
		PacketQueue& operator=(const PacketQueue&);

	public:
		PacketQueue();
		void addPacket(Packet * aPacket) throw (InvalidPacketType);
        bool empty();
		Packet * getPacket(); //Gives you back the first packet in the queue and destroys it
		std::recursive_mutex& getMutex();
		

};

#endif
