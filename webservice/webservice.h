/*
 *  Created by Roel Storms
 *
 *  This class uses the mongoose library to created a web service using HTTP on port 80 or HTTPS.
 *  Privileged users on the client can send request to this webservice to changed the ZigBee
 *  network configuration or parameters. Correct incoming requests will result in the creation of
 *  WSPackets which will be pushed onto the shared queue to the main thread.
 *
 *  The url of the request specifies the type of request and the XML data specify all the necessary
 *  data to fulfil this request. In the URL an authentication code is used. Together with HTTPS this
 *  should prevent unwanted access to the gateway.
 *
 */

#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <stdio.h>
#include <string.h>
#include "mongoose.h"
#include "wspacket.h"
#include "../packetqueue.h"
#include <mutex>
#include <condition_variable>
#include "XML/XML.h"
#include "wsaddnodepacket.h"
#include "wsaddsensorspacket.h"
#include "wschangefrequencypacket.h"
#include "wsrequestdatapacket.h"

class Webservice
{
	private:
	struct mg_context *ctx;

    PacketQueue * wsReceiveQueue, * wsSendQueue;
    std::condition_variable * mainConditionVariable, * webserviceConditionVariable;
    std::mutex * mainConditionVariableMutex, * webserviceConditionVariableMutex;

    public:
    Webservice(PacketQueue * wsReceiveQueue, PacketQueue * wsSendQueue, std::condition_variable * mainConditionVariable, std::mutex * mainConditionVariableMutex, std::condition_variable * webserviceConditionVariable, std::mutex * webserviceConditionVariableMutex);
	~Webservice();
	static int beginRequestHandlerWrapper(struct mg_connection *conn);
	int beginRequestHandler(struct mg_connection *conn);

    std::string login(std::string, std::string);
};

#endif
