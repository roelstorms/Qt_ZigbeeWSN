#include "connection.h"

Connection::Connection()
{
	std::cout << "connection constructor" << std::endl;
}

Connection::~Connection()
{
	closeConnection();
}

std::vector <int> Connection::listPortNumbers()
{
	std::vector<int> list;

	return list;
}

int Connection::openPort(int portNumber, int baudrate)
{
	std::string device("/dev/ttyUSB");
	device.append(std::to_string(portNumber));
	
	std::cout << "device : " << device << std::endl;
	const char * address = device.c_str();
	connectionDescriptor = open(address , O_RDWR | O_NOCTTY | O_NDELAY);
	std::cout << "connectionDescriptor: " << connectionDescriptor << std::endl;
 	if (connectionDescriptor == -1)
 	{
		/*
		 * Could not open the port.
		 */

		std::cerr << "open_port: Unable to open " << address << std::endl;
		throw SerialError();
	}
   	else
    	{
		std::cout << "set up connection" << std::endl;
        //fcntl(connectionDescriptor, F_SETFL, FNDELAY);		// Set the port to unblocking
        fcntl(connectionDescriptor, F_SETFL, 0);		// Set the port to blocking
    	}

    	struct termios options;
    	tcgetattr(connectionDescriptor, &options);

	//options.c_cflag |= (CLOCAL | CREAD);
	switch (baudrate)
	{
		case 1200:
		options.c_cflag = B1200;		//Set baudrate to 9600 baud
		break;
		case 2400:
		options.c_cflag = B2400;		//Set baudrate to 9600 baud
		break;
		case 4800:
		options.c_cflag = B4800;		//Set baudrate to 9600 baud
		break;
		case 9600:
		options.c_cflag = B9600;		//Set baudrate to 9600 baud
		break;
		case 19200:
		options.c_cflag = B19200;		//Set baudrate to 9600 baud
		break;
		case 38400:
		options.c_cflag = B38400;		//Set baudrate to 9600 baud
		break;
		case 57600:
		options.c_cflag = B57600;		//Set baudrate to 9600 baud
		break;
		case 115200:
		options.c_cflag = B115200;		//Set baudrate to 9600 baud
		break;
	}

	options.c_cflag = B9600;		//Set baudrate to 9600 baud
	options.c_cflag |= CS8;	
	options.c_cflag |= CLOCAL;
	// no parity 8N1
	options.c_cflag &= ~PARENB;		// Set parity bit
	options.c_cflag &= ~CSTOPB;		// Set Stopbit
	options.c_cflag &= ~CSIZE;		
	options.c_cflag |= CREAD;		
	options.c_cflag |= CS8;		
	options.c_oflag = 0;
	options.c_lflag = 0;
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 1;
	tcsetattr(connectionDescriptor , TCSANOW, &options);
	tcflush(connectionDescriptor, TCIFLUSH);
	tcflush(connectionDescriptor, TCOFLUSH);  
	
	return connectionDescriptor;
}

void Connection::closeConnection()
{	
	std::cout << "Closing connection file descriptor" << std::endl;
	close(connectionDescriptor);
}

int Connection::getConnectionDescriptor()
{
	return connectionDescriptor;
}
