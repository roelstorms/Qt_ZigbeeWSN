#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <stdio.h>
#include <string.h>
#include "mongoose.h"
#include "wspacket.h"
#include "../packetqueue.h"
#include <mutex>
#include <condition_variable>

class Webservice
{
	private:
	struct mg_context *ctx;
	struct mg_callbacks callbacks;
	std::condition_variable * mainConditionVariable;
	std::mutex * mainConditionVariableMutex;
		// List of options. Last element must be NULL.
    PacketQueue * wsQueue;


    public:
    Webservice(PacketQueue * aWSQueue, std::condition_variable * aWSConditionVariable, std::mutex * aWSConditionVariableMutex);
	~Webservice();
	void operator() ();	
	static int beginRequestHandlerWrapper(struct mg_connection *conn);
	int beginRequestHandler(struct mg_connection *conn);

};

#endif
