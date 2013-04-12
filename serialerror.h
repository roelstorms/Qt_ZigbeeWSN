#ifndef SERIALERROR_H
#define SERIALERROR_H
#include <exception>

class SerialError : public std::exception
{

	public :
	
		const char* what() const noexcept
		{
			return "Couldn't open serial port, check if the device is connected correctly and if the port number is right"; 
		}


};

#endif
