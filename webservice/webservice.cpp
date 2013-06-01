#include "webservice.h"


Webservice::Webservice(PacketQueue * wsReceiveQueue, PacketQueue * wsSendQueue, std::condition_variable * mainConditionVariable, std::mutex * mainConditionVariableMutex, std::condition_variable * webserviceConditionVariable, std::mutex * webserviceConditionVariableMutex) :
    wsReceiveQueue(wsReceiveQueue), wsSendQueue(wsSendQueue), mainConditionVariable(mainConditionVariable), mainConditionVariableMutex(mainConditionVariableMutex), webserviceConditionVariable(webserviceConditionVariable), webserviceConditionVariableMutex(webserviceConditionVariableMutex)
{
    //const char *options[] = {"listening_ports", "8080s", "ssl_certificate",  "../server.pem","error_log_file", "./webservice_error.txt", NULL};
    const char *options[] = {"listening_ports", "80", "error_log_file", "./webservice_error.txt", NULL};
    #ifdef WS_DEBUG
        std::cout << "begin of Webservice constructor" << std::endl;
    #endif
    struct mg_callbacks callbacks;

    // Prepare callbacks structure. We have only one callback, the rest are NULL.
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = &Webservice::beginRequestHandlerWrapper;

    // Start the web server.
    ctx = mg_start(&callbacks, (void*) this, options);
    #ifdef WS_DEBUG
        std::cout << "end of Webservice constructor" << std::endl;
    #endif
}

Webservice::~Webservice()
{
    // Stop the server.
    mg_stop(ctx);

}

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
    // int post_data_len = mg_read(conn, post_data, sizeof(post_data));    // warning since post_data_len is never used, maybe used in the future.

	// Prepare the message we're going to send
    #ifdef WS_DEBUG
        std::cout << std::endl << "url: "<< request_info->uri << std::endl;
    #endif
	Packet * packet; 
    std::string url(request_info->uri);
    #ifdef WS_DEBUG
        std::cout << "data: " << std::endl << post_data << std::endl;
    #endif
    if(url.find("9e7e3dccf2bfb4d927eec92d7a896655") == std::string::npos)
    {
        int content_length = snprintf(content, sizeof(content),	"<error>invalid key</error>");
        mg_printf(conn,"HTTP/1.1 401 Unauthorized\r\n"
                  "Content-Type: text/plain\r\n"
                  "Content-Length: %d\r\n"        // Always set Content-Length
                  "\r\n"
                  "%s",
                  content_length, content);
        return 1;
    }

    try{
        if(url.find("changeFrequency") != std::string::npos)
        {
            #ifdef WS_DEBUG
                std::cout << "request type set to CHANGE_FREQUENCY" << std::endl;
            #endif
            packet = dynamic_cast<Packet *> (new WSChangeFrequencyPacket(std::string(post_data)));
        }
        else if(url.find("addNode") != std::string::npos)
        {
            #ifdef WS_DEBUG
                std::cout << "request type set to ADD_NODE" << std::endl;
            #endif
            try
            {
                packet = dynamic_cast<Packet *> (new WSAddNodePacket(std::string(post_data)));

            }
            catch (InvalidXMLError)
            {
                std::cerr << "Tried to create a WSAddNodePacket in a WS Thread but XML was invalid" << std::endl;
            }
        }
        else if(url.find("addSensor") != std::string::npos)
        {
            #ifdef WS_DEBUG
                std::cout << "request type set to ADD_SENSOR" << std::endl;
            #endif
            packet = dynamic_cast<Packet *> (new WSAddSensorsPacket(std::string(post_data)));
        }
        else if(url.find("requestData") != std::string::npos)
        {
            #ifdef WS_DEBUG
                std::cout << "request type set to ADD_SENSOR" << std::endl;
            #endif
            packet = dynamic_cast<Packet *> (new WSRequestDataPacket(std::string(post_data)));
        }
        else
        {
            std::cerr << "invalid command was sent to the webservice" << std::endl;
            int content_length = snprintf(content, sizeof(content),	"<error>Invalid URL</error>");
            mg_printf(conn,"HTTP/1.1 405 Method Not Allowed\r\n"
                      "Content-Type: text/plain\r\n"
                      "Content-Length: %d\r\n"        // Always set Content-Length
                      "\r\n"
                      "%s",
                      content_length, content);
            return 1;
        }
    }
    catch(InvalidWSXML)
    {
        int content_length = snprintf(content, sizeof(content),	"<error>invalid XML</error>");
        mg_printf(conn,"HTTP/1.1 405 Method Not Allowed\r\n"
                  "Content-Type: text/plain\r\n"
                  "Content-Length: %d\r\n"        // Always set Content-Length
                  "\r\n"
                  "%s",
                  content_length, content);
        return 1;
    }
    catch(InvalidXMLError)
    {
        int content_length = snprintf(content, sizeof(content),	"<error>invalid XML</error>");
        mg_printf(conn,"HTTP/1.1 405 Method Not Allowed\r\n"
                  "Content-Type: text/plain\r\n"
                  "Content-Length: %d\r\n"        // Always set Content-Length
                  "\r\n"
                  "%s",
                  content_length, content);
        return 1;
    }
	
    wsReceiveQueue->addPacket(packet);
    #ifdef WS_DEBUG
        std::cout << "adding ws packet to wsqueue" << std::endl;
    #endif
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

