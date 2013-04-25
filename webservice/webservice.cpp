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
	int post_data_len;
	char post_data[1024] = "";

	// Prepare the message we're going to send
	post_data_len = mg_read(conn, post_data, sizeof(post_data));
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
		return 1;	
	}
	
    wsQueue->addPacket(packet);
	std::cout << "adding ws packet to wsqueue" << std::endl;
	std::lock_guard<std::mutex> lg(*mainConditionVariableMutex);
	mainConditionVariable->notify_all();

	int content_length = snprintf(content, sizeof(content),	"<error>false</error>",request_info->uri, post_data);
		printf("%s: %d\n", post_data, content_length);
	
	// Send HTTP reply to the client
	mg_printf(conn,
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: %d\r\n"        // Always set Content-Length
			"\r\n"
			"%s",
			content_length, content);

	// Returning non-zero tells mongoose that our function has replied to
	// the client, and mongoose should not send client any more data.
	return 1;

}

std::string Webservice::login(std::string , std::string )
{
    /*
    XML XMLParser;
    xercesc::DOMDocument * doc = XMLParser.parseToDom(data);

    xercesc::DOMElement * docElement = doc->getDocumentElement();
    xercesc::DOMElement * nextElement;
    nextElement = docElement->getFirstElementChild();

    int sensorGroupID = -1;

    XMLCh * usernameString = xercesc::XMLString::transcode("username");
    XMLCh * passwordIDString = xercesc::XMLString::transcode("password");


    std::vector<std::pair<int, int> > frequencies;      // IpsumID + frequency (interval) in seconds

    while(nextElement != NULL)
    {
        if(xercesc::XMLString::compareIString(nextElement->getTagName(), sensorGroupIDString) == 0)
        {
            temp = xercesc::XMLString::transcode(nextElement->getTextContent());
            sensorGroupID = boost::lexical_cast<int>(std::string(temp));
            xercesc::XMLString::release(&temp);

        }
        if(xercesc::XMLString::compareIString(nextElement->getTagName(), sensorString) == 0)
        {
            std::pair<int, int> freq;
            xercesc::DOMElement * child;
            child = nextElement->getFirstElementChild();

            if(xercesc::XMLString::compareIString(child->getTagName(), sensorIDString) == 0)
            {
                temp = xercesc::XMLString::transcode(child->getTextContent());
                freq.first = boost::lexical_cast<int>(temp);
                xercesc::XMLString::release(&temp);

            }
            else
            {
                throw InvalidWSXML();
            }

            child = child->getNextElementSibling();

            if(xercesc::XMLString::compareIString(child->getTagName(), frequencyString) == 0)
            {
                temp = xercesc::XMLString::transcode(child->getTextContent());
                freq.second = boost::lexical_cast<int> (temp);

                xercesc::XMLString::release(&temp);
            }
            else
            {
                throw InvalidWSXML();
            }
            std::cout << "adding sensor to vector" << std::endl;
            frequencies.push_back(freq);
        }
        else
        {
            std::cerr << "invalid XML" << std::endl;
            throw InvalidWSXML();
        }

        nextElement = nextElement->getNextElementSibling();
    }
    */
}



Webservice::Webservice(PacketQueue * aWSQueue, std::condition_variable * mainConditionVariable, std::mutex * mainConditionVariableMutex) : wsQueue(aWSQueue), mainConditionVariable(mainConditionVariable), mainConditionVariableMutex(mainConditionVariableMutex)
{
	const char *options[] = {"listening_ports", "8080", "error_log_file", "./webservice_error.txt", NULL};
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
