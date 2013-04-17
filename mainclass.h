#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <fcntl.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <boost/thread.hpp>
#include "./SerialCom/ZBReceiver.h"
#include "./SerialCom/ZBSender.h"
#include "./SerialCom/connection.h"
#include "./SerialCom/packets/libeliopacket.h"
#include "./SerialCom/packets/libelmaskrequest.h"
#include "./SerialCom/packets/libelmaskresponse.h"
#include "./SerialCom/packets/libeladdnoderesponse.h"
#include "./SerialCom/packets/libeladdnodepacket.h"
#include "./SerialCom/packets/libelchangefreqpacket.h"
#include "./SerialCom/packets/libelchangefreqresponse.h"
#include "./SerialCom/packets/libelchangenodefreqpacket.h"
#include "./SerialCom/packets/libelchangenodefreqresponse.h"

#include "./HTTP/http.h"
#include "packetqueue.h"
#include "sentpackets.h"

#include "./webservice/webservice.h"
#include <thread>
#include "./HTTP/ipsum.h"
#include <ctime>
#include <typeinfo>

#include "sqlite/sql.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>


template <class P, class R> // P for packet, R for response

class SentPackets
{
private:
    std::vector<std::pair<P, int> > sentPackets;     // Packet and timestamp
public:
    SentPackets();
    void addPacket(P packet);
    P  retrieveCorrespondingPacket(R packet);

    std::vector<P> findExpiredPacket(int expirationTime);
};

template <class P, class R>
SentPackets<P,R>::SentPackets()
{
    std::cout << "SentZBPackets<P,R>::SentZBPackets()" << std::endl;
}

template <class P, class R>
void SentPackets<P, R>::addPacket(P packet)
{
    int currentTime = time(NULL);
    sentPackets.push_back(std::pair<P, int> (packet, currentTime));

    std::cout << "Packet added to sentZBPacket with time: " << currentTime << std::endl;
}

template <class P, class R>
P SentPackets<P, R>::retrieveCorrespondingPacket(R packet)
{
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if(packet->correspondsTo(it->first))
        {
            return it->first;
        }
    }
}

template <class P, class R>
std::vector<P> SentPackets<P, R>::findExpiredPacket(int expirationTime)
{
    std::vector<P*> expiredPackets;

    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if(it->second > expirationTime)
        {
            expiredPackets.push_back(it->first);
            sentPackets.erase(it);
        }
    }
    return expiredPackets;
}





class MainClass
{
	private:
	Http * socket;
	
    SentPackets<LibelAddNodePacket *, LibelAddNodeResponse *>  * addNodeSentPackets;

	PacketQueue * zbReceiveQueue, * zbSenderQueue, * wsQueue, * ipsumSendQueue, * ipsumReceiveQueue;
    std::queue<Packet *> * localZBReceiveQueue, * localWSQueue, * localIpsumSendQueue, * localIpsumReceiveQueue, * sentZBPackets;

	std::condition_variable * mainConditionVariable, * ipsumConditionVariable, * zbSenderConditionVariable;
	std::mutex * conditionVariableMutex, * ipsumConditionVariableMutex, * zbSenderConditionVariableMutex;

	Connection * con;
	Sql * db;

	boost::thread * zbReceiverThread, * zbSenderThread, * ipsumThread;
	ZBReceiver * zbReceiver;
	ZBSender * zbSender;
	Webservice * webService;
	Ipsum * ipsum;


	public:
	MainClass(int argc, char * argv[]);
	~MainClass();
	void operator () ();

	void libelIOHandler(Packet * packet);
	void libelMaskResponseHandler(Packet * packet);
	void libelChangeFreqResponseHandler(Packet * packet);
	void libelChangeNodeFreqResponseHandler(Packet * packet);
	void libelAddNodeResponseHandler(Packet * packet);
		
	void webserviceHandler(Packet * packet);
    void requestIOHandler(WSPacket * wsPacket) throw (InvalidWSXML);
    void changeFrequencyHandler(WSPacket * wsPacket) throw (InvalidWSXML);
    void addNodeHandler(WSPacket * wsPacket) throw (InvalidWSXML);
    void addSensorHandler(WSPacket * wsPacket) throw (InvalidWSXML);


    SensorType stringToSensorType(std::string sensorType) throw (InvalidWSXML) ;
};

#endif
