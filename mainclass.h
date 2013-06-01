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
#include "./SerialCom/packets/libelerrorpacket.h"


#include "webservice/wsaddsensorspacket.h"
#include "webservice/wsaddnodepacket.h"
#include "webservice/wschangefrequencypacket.h"
#include "webservice/wsrequestdatapacket.h"

#include "./HTTP/http.h"
#include "./HTTP/ipsum.h"
#include "./HTTP/ipsumchangefreqpacket.h"
#include "./HTTP/ipsumchangeinusepacket.h"
#include "./HTTP/ipsumuploadpacket.h"

#include "packetqueue.h"
#include "sentpackets.h"

#include "./webservice/webservice.h"
#include <thread>
#include <ctime>
#include <typeinfo>

#include "sqlite/sql.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>


#include "config.h"

class Sql;

//extern std::map<SensorType, std::string> sensorMap;

class MainClass
{
	private:
    int packetExpirationTime;

    unsigned char nextFrameID;

	Http * socket;
	
    SentPackets<LibelAddNodePacket *, LibelAddNodeResponse *>  * addNodeSentPackets;
    SentPackets<LibelChangeFreqPacket *, LibelChangeFreqResponse *>  * changeFreqSentPackets;

    PacketQueue * zbReceiveQueue, * zbSenderQueue, * wsSendQueue, * wsReceiveQueue, * ipsumSendQueue, * ipsumReceiveQueue;
    std::queue<Packet *> * localZBReceiveQueue, * localWSReceiveQueue, * localIpsumSendQueue, * localIpsumReceiveQueue, * sentZBPackets;
    std::vector<Packet *> * localZBSenderQueue;
    std::condition_variable * mainConditionVariable, * ipsumConditionVariable, * zbSenderConditionVariable, * wsConditionVariable;
    std::mutex * conditionVariableMutex, * ipsumConditionVariableMutex, * zbSenderConditionVariableMutex, * wsConditionVariableMutex;

	Connection * con;
	Sql * db;

	boost::thread * zbReceiverThread, * zbSenderThread, * ipsumThread;
	ZBReceiver * zbReceiver;
	ZBSender * zbSender;
	Webservice * webService;
	Ipsum * ipsum;

    bool exit;

    //static std::map<SensorType, std::string> sensorMap;


    /*
     * A function that converts std::string to std::vector<unsigned char>, mainly used to translate the string retrieved from
     * the sqlite DB to a vector of unsigned chars that is needed by the libelium packets.
     */
    std::vector<unsigned char> convertStringToVector(std::string input);

    /*
     *  Handles incoming IO packets from the zigbee network. These packets come from the ZBReceiver thread. Each incoming packet is analyzed and generates
     *  and ipsum packet to upload this data to ipsum. Also locally stored packets for a zigbee node are sent out when a data packet is received.
     *  This is done because only at this moment we know that the zigbee node is not sleeping and will receive packets from the gateway.
     *
     */
    void libelIOHandler(LibelIOPacket * libelIOPacket);
    void libelMaskResponseHandler(LibelMaskResponse * libelMaskResponse);
    void libelChangeFreqResponseHandler(LibelChangeFreqResponse * libelChangeFreqResponse);
    void libelChangeNodeFreqResponseHandler(LibelChangeNodeFreqResponse * libelChangeNodeFreqResponse);
    void libelAddNodeResponseHandler(LibelAddNodeResponse * libelAddNodeResponse);
    void transmitStatusHandler(TransmitStatusPacket *transmitStatusPacket);

    void requestDataHandler(WSRequestDataPacket *  wsRequestDataPacket);
    void changeFrequencyHandler(WSChangeFrequencyPacket *  wsChangeFrequencyPacket);
    void addNodeHandler(WSAddNodePacket * wsAddNodePacket);
    void addSensorHandler(WSAddSensorsPacket * wsAddSensorsPacket);

    void checkExpiredPackets();
    std::string ucharVectToString(const std::vector<unsigned char> &ucharVect);
    unsigned char getNextFrameID();



    public:


    MainClass(int argc, char * argv[], int packetExpirationTime, unsigned char numberOfRetries) throw (StartupError);
	~MainClass();
	void operator () ();

    /*
    static std::map<SensorType, std::string> getSensorMap()
    {
        return MainClass::sensorMap;
    }
*/
};


#endif
