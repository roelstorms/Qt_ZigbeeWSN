#include "webservice.h"

// This function will be called by mongoose on every new request.
int Webservice::beginRequestHandlerWrapper(struct mg_connection *conn)
{
	const struct mg_request_info *request_info = mg_get_request_info(conn);
	
	return static_cast<Webservice*>(request_info->user_data)->beginRequestHandler(conn);
}

int Webservice::beginRequestHandler(struct mg_connection *conn)
{
	const struct mg_request_info *request_info = mg_get_request_info(conn);

	char content[500];
	char post_data[1024] = "";

	// Prepare the message we're going to send
	std::cout << std::endl << "url: "<< request_info->uri << std::endl;
	Packet * packet; 
    std::string url(request_info->uri);

    /*
    if(url.find("login") != std::string::npos)
    {
        login(std::string(post_data), url);
    }
    */

	try
	{
		packet= /*dynamic_cast<Packet *>*/ (new WSPacket(std::string(request_info->uri), std::string(post_data)));
		std::cout << "type of WSPacket* in webservice.cpp: " << typeid(packet).name() << std::endl;
	}
	catch (WebserviceInvalidCommand)
	{
		std::cerr << "invalid command was sent to the webservice" << std::endl;

        mg_printf(conn,"HTTP/1.1 405 Method Not Allowed\r\n"
                  "Content-Type: text/plain\r\n"
                  "Content-Length: 19\r\n"        // Always set Content-Length
                  "\r\n"
                  "<error>True</error>"
                  );
		return 1;	
	}
	
    wsReceiveQueue->addPacket(packet);
	std::cout << "adding ws packet to wsqueue" << std::endl;
	std::lock_guard<std::mutex> lg(*mainConditionVariableMutex);
	mainConditionVariable->notify_all();

    int content_length = snprintf(content, sizeof(content),	"<error>false</error>");//,request_info->uri, post_data);
		printf("%s: %d\n", post_data, content_length);
	
	// Send HTTP reply to the client
	mg_printf(conn,
            "HTTP/1.1 202 Accepted\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: %d\r\n"        // Always set Content-Length
			"\r\n"
			"%s",
			content_length, content);

	// Returning non-zero tells mongoose that our function has replied to
	// the client, and mongoose should not send client any more data.
	return 1;

}


Webservice::Webservice(PacketQueue * wsReceiveQueue, PacketQueue * wsSendQueue, std::condition_variable * mainConditionVariable, std::mutex * mainConditionVariableMutex, std::condition_variable * webserviceConditionVariable, std::mutex * webserviceConditionVariableMutex) : wsReceiveQueue(wsReceiveQueue), wsSendQueue(wsSendQueue), mainConditionVariable(mainConditionVariable), mainConditionVariableMutex(mainConditionVariableMutex), webserviceConditionVariable(webserviceConditionVariable), webserviceConditionVariableMutex(webserviceConditionVariableMutex)
{
    const char *options[] = {"listening_ports", "8080s", "ssl_certificate",  "../server.pem","error_log_file", "./webservice_error.txt", NULL};
    //const char *options[] = {"listening_ports", "8080", "error_log_file", "./webservice_error.txt", NULL};
    std::cout << "begin of Webservice constructor" << std::endl;
    struct mg_callbacks callbacks;

	// Prepare callbacks structure. We have only one callback, the rest are NULL.
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.begin_request = &Webservice::beginRequestHandlerWrapper;

	// Start the web server.
	ctx = mg_start(&callbacks, (void*) this, options);
	std::cout << "end of Webservice constructor" << std::endl;
}

Webservice::~Webservice()
{
	// Stop the server.
	mg_stop(ctx);

}

void Webservice::operator() ()
{
	
}
