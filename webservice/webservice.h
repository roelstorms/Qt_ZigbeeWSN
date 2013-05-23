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
    Webservice( PacketQueue * wsReceiveQueue, PacketQueue * wsSendQueue, std::condition_variable * mainConditionVariable, std::mutex * mainConditionVariableMutex, std::condition_variable * webserviceConditionVariable, std::mutex * webserviceConditionVariableMutex);
	~Webservice();
	static int beginRequestHandlerWrapper(struct mg_connection *conn);
	int beginRequestHandler(struct mg_connection *conn);

    std::string login(std::string, std::string);
};

#endif
