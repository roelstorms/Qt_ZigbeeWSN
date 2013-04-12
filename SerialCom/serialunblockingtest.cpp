/*
 *	Created by Roel Storms on 28/01/2013  
 *	
 * 	compile : g++ main.cpp -o main.out -lboost_thread inputhandler.cpp
 *	sudo chmod 666 /dev/ttyUSB0 to allow acces to the serial port
 *
 */

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
#include "inputhandler.h"
#include "connection.h"
#include "userinput.h"
#include <stropts.h>

unsigned char readByte(int fd)
{
	int input = 0x0;
	int count = read(fd, &input, 1);
	if (count > 0)
	{
		printf("%X\n", input);
	}
	//ioctl(fd, I_FLUSH, FLUSHR); //Flush read and write stream
	return input;
}


int main(int argc, char* argv[])
{

	int fd; /* File descriptor for the port */
	unsigned char input = 0x0;

	Connection con(argv[1]);
	fd = con.getConnectionDescriptor();
	while(true)
	{
		input = readByte(fd);
		
	}

	return 0;
}

