/*
 *	By Roel Storms
 *
 *	A simple program that sends an AT command (2nd argument) to the node connected to the computer on port specified by the 1st argument
 *	and then waits for the node to reply
 *	
 */



#include "../APIFrames.h"	
#include "../connection.h"
#include "../inputhandler.h"
#include "unistd.h"
#include "stdlib.h"

int main(int argc, char* argv[])
{
	
	if (argc != 3)
	{
		std::cout << "not enough arguments" << std::endl;
		return 1;
	}
	Connection con;
	con.openPort(atoi(argv[1]));
	std::vector<unsigned char> ATCommand;
	std::vector<unsigned char> empty;

	int connectionDescriptor = con.getConnectionDescriptor();
	bool b = true;
	InputHandler ih(connectionDescriptor, &b);

	ATCommand.push_back(argv[2][0]);
	ATCommand.push_back(argv[2][1]);
	con.sendPacket( APIFrames::ATCommand(ATCommand, empty));

	char input;
	while(true)
	{

		input = ih.readByte(connectionDescriptor);
		//if(input != 0X0) printf("%X", input);
	}

	std::cout << "end" << std::endl;
	con.closeConnection();
	return 0;
}
