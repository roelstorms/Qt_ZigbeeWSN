#include "webservice.h"

int main(int argc, char** argv)
{
	std::queue<WSPacket> webserviceRequestQueue;
	std::mutex webserviceRequestMutex;

	Webservice webservice(&webserviceRequestQueue, &webserviceRequestMutex);

	// Wait until user hits "enter". Server is running in separate thread.
	// Navigating to http://localhost:8080 will invoke begin_request_handler().
	getchar();
	return 0;
}
