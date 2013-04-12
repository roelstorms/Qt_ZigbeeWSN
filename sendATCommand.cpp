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
#include "SerialCom/connection.h"
#include "SerialCom/APIFrames.h"

int main(int argc, char* argv[])
{

	int fd; /* File descriptor for the port */
	int i;
	std::cout << "argc: " << argc << std::endl;
	if(argc != 3)
	{
		std::cout << "also provide the port number" << std::endl;
		return 1;
	}
	Connection con;
     	

	bool stop = false;
	int connectionDescriptor = con.openPort(atoi(argv[1]), 9600);

	std::cout << "AT command: " << argv[2] << std::endl;	
	std::vector<unsigned char> ATCommand, empty, output;
	ATCommand.push_back(argv[2][0]);
	ATCommand.push_back(argv[2][1]);

	con.sendPacket(APIFrames::ATCommand(ATCommand, empty, false));
	
	char input;
	
	std::cout << "AT command sent" << std::endl;
	while(true)
	{
		if(read(connectionDescriptor, (void*) &input, 1) > 0)
		{
			printf("%X", input);	
		}
	}
	return 0;
}

