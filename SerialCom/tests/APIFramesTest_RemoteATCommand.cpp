#include "../APIFrames.h"	
#include "../connection.h"
#include "unistd.h"
#include "stdlib.h"

int main(int argc, char* argv[])
{
	
	Connection con(argv[1]);
	con.startInputHandler();
	std::vector<unsigned char> ATCommand;
	std::vector<unsigned char> data;
	std::vector<unsigned char> networkAddress;
	std::vector<unsigned char> destinationAddress;
	
	ATCommand.push_back('B');
	ATCommand.push_back('H');
	data.push_back(0x01);

	networkAddress.push_back(0x74);
	networkAddress.push_back(0xDE);
	destinationAddress.push_back(0X00);	
	destinationAddress.push_back(0X13);	
	destinationAddress.push_back(0XA2);	
	destinationAddress.push_back(0X00);	
	destinationAddress.push_back(0X40);	
	destinationAddress.push_back(0x79);	
	destinationAddress.push_back(0x5B);	
	destinationAddress.push_back(0xFE);	
	//data.at(1) = 0x0B;
	con.sendPacket( APIFrames::remoteATCommand(ATCommand, data, destinationAddress, networkAddress));
	
	std::cout << std::endl << "end" << std::endl << std::endl;
	con.closeConnection();
	return 0;
}
