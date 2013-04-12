#include "../enums.h"
#include "packets/libelchangefreqpacket.h"
#include "packets/libeliopacket.h"
#include "packets/libeladdnodepacket.h"
#include "packets/libelrequestiopacket.h"
#include "packets/libeladdnoderesponse.h"
#include "ZBSender.h"
#include <mutex>
#include <condition_variable>
#include "connection.h"
#include "../packetqueue.h"
#include <boost/thread.hpp>
#include <vector>
#include <cmath>

int main(int argc, char ** argv)
{
	Connection con;	

	std::mutex zbSenderConditionVariableMutex;
	std::condition_variable zbSenderConditionVariable;
	PacketQueue zbSendQueue;
	ZBSender * zbSender = new ZBSender(con.openPort(atoi(argv[1]), 9600), &zbSenderConditionVariableMutex, &zbSenderConditionVariable, &zbSendQueue);

	
	boost::thread * zbSenderThread = new boost::thread(boost::ref(*zbSender));



	std::vector<unsigned char> zigbeeAddress64bit{0X00, 0X13, 0XA2, 0X00, 0X40, 0X69, 0X73, 0X77};
	std::vector<SensorType> sensors{TEMP, PRES, BAT};

	for(int i = 0; i < 20; ++i)
	{
		LibelAddNodePacket * libelAddNodePacket = new LibelAddNodePacket(zigbeeAddress64bit, sensors);	 	
		zbSendQueue.addPacket(dynamic_cast<Packet *> (libelAddNodePacket));
		zbSenderConditionVariableMutex.lock();
		zbSenderConditionVariable.notify_all();
		zbSenderConditionVariableMutex.unlock();
		std::cout << "packet sent" << std::endl;
		sleep(5);

	}
	zbSenderThread->join();
	delete zbSender;
	delete zbSenderThread;
	return 0;
}

