
#include <string>
#include <boost/thread.hpp>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include <fstream>

#include "../connection.h"
#include "unistd.h"
#include "stdlib.h"
#include "../dataIOPacket.h"

unsigned char readByte(int fd)
{
	int input = 0x0;
	read(fd, &input, 1);
	if(input == 0x7D)
	{
		read(fd, &input, 1);
		std::cout << "input: " << std::hex << input << std::endl;	
		input = input ^ 0x20;
		std::cout << "XOR'ed input: " << std::hex << input << std::endl;	
	}
	return input;
}


int main(int argc, char* argv[])
{
	
	Connection con(argv[1]);
	//con.startInputHandler();

	int fileDescriptor = con.getConnectionDescriptor();
	
	unsigned char input = 0x0;
	std::vector<unsigned char> inputVector;
	while(1)
    	{
		input = readByte(fileDescriptor);
		if(input == 0x7E)
		{
			inputVector.push_back(input);
			input = readByte(fileDescriptor);
			inputVector.push_back(input);
			int packetSize = input * 256;

			input = readByte(fileDescriptor);
			inputVector.push_back(input);
			packetSize += input;	
			

			
			printf("pSize: %d\n", packetSize);
			for(int position = 0; position <= packetSize; ++position)
			{
				input = readByte(fileDescriptor);
				inputVector.push_back(input);
			}
			if(inputVector.at(3) == 0x92)
			{
				DataIOPacket dp(inputVector);
				inputVector.erase(inputVector.begin(), inputVector.end());
				std::cout << "reading analog pin 0: " << dp.readAnalog(0) << std::endl;
 
			}
		}
	}


	con.closeConnection();
	return 0;
}
