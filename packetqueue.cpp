#include "packetqueue.h"

PacketQueue::PacketQueue() 
{

}

void PacketQueue::addPacket(Packet * aPacket) throw (InvalidPacketType)
{
	std::cout << "adding packet to queue" << std::endl;
	std::lock_guard<std::recursive_mutex> lg(mutex);
	queue.push(aPacket);
	std::cout << "packet added" << std::endl;

}

bool PacketQueue::empty() 
{
	std::lock_guard<std::recursive_mutex> lg(mutex);
	return queue.empty();
}

Packet * PacketQueue::getPacket() 
{
	std::lock_guard<std::recursive_mutex> lg(mutex);

	Packet * output = queue.front();
	queue.pop();
	return output;
}

std::recursive_mutex& PacketQueue::getMutex()
{
	return mutex;
}
