#include "ZBSender.h"

ZBSender::ZBSender(int connectionDescriptor, std::mutex * zbSenderConditionVariableMutex, std::condition_variable * zbSenderConditionVariable, PacketQueue * zbSendQueue) : connectionDescriptor(connectionDescriptor), zbSenderConditionVariableMutex(zbSenderConditionVariableMutex), zbSenderConditionVariable(zbSenderConditionVariable), zbSendQueue(zbSendQueue)
{
	std::cout << "ZBSender constructor" << std::endl;
}

std::vector<unsigned char> ZBSender::escape(std::vector<unsigned char> data)
{
	std::vector<unsigned char> escapedData;
	std::cout << std::endl << "escaping packet" << std::endl;
	escapedData.push_back(0X7E);
	for(auto it = data.begin() + 1; it < data.end(); ++it)
	{
		if ((*it) == 0X7E || (*it) == 0X7D || (*it) == 0X11 || (*it) == 0X13)
		{
			escapedData.push_back(0x7D);
			escapedData.push_back((*it) ^ 0x20);
		}
		else
		{
			escapedData.push_back(*it);
		}
	}
	for(auto it = escapedData.begin(); it < escapedData.end(); ++it)
	{
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it) << " ";
	}
	return escapedData;
}

void ZBSender::operator() ()
{
	while(true)
	{
        std::unique_lock<std::mutex> uniqueLock(*zbSenderConditionVariableMutex);
        zbSenderConditionVariable->wait(uniqueLock, [this]{return (!zbSendQueue->empty());});
        //std::cout << "zb sender out of wait" << std::endl;
		ZBPacket * packet;

		while(!zbSendQueue->empty())
		{
			//if(typeid(packet) ==  typeid(LibelAddNodePacket *) || typeid(packet) ==  typeid(LibelRequestIOPacket *) || typeid(packet) ==  typeid(LibelChangeFreqPacket *) || typeid(packet) ==  typeid(LibelChangeNodeFreqPacket *) || typeid(packet) ==  typeid(LibelMaskRequest *) )
			{
				std::cout << "Sendable packet received in ZBSender" << std::endl;
				packet = dynamic_cast<ZBPacket *> (zbSendQueue->getPacket());
				std::cout << "sending : " << *packet << std::endl;
				auto data = escape(packet->getEncodedPacket());
                write(connectionDescriptor, (void*) data.data(),  data.size());
                fsync(connectionDescriptor);

                delete packet;
			}
		}
	}

}
