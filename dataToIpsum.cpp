/*
 * Created by Roel Storms on 06/03/2013
 * This program will combine serial zigbee connection to get data out of the zigbee WSN and push it to the ipsum database using HTTP post request.
 *
 */

#include "./XML/XML.h"
#include "./HTTP/http.h"
#include "./SerialCom/inputhandler.h"
#include "./SerialCom/connection.h"
#include "./SerialCom/userinput.h"
#include "DataHandler.h"


int main(int argc, char * argv[])
{


	int fd; /* File descriptor for the port */
	int i;
	char out[255], input;
	int status;

	Connection con(argv[1]);

	bool stop = false;
	int connectionDescriptor = con.getConnectionDescriptor();
	DataHandler dh();
	InputHandler inputHandler(connectionDescriptor, &stop);
	boost::thread inputThread(boost::ref(inputHandler));

	
	inputThread.join();

	return 0;
}
