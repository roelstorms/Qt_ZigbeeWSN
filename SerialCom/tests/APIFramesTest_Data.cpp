#include "../APIFrames.h"	
#include "../connection.h"
#include "unistd.h"
#include "stdlib.h"

int main(int argc, char* argv[])
{
	
	std::string input = argv[2];
	Connection con(argv[1]);
	con.startInputHandler();
	std::vector<unsigned char> ATCommand;
	std::vector<unsigned char> data(input.begin(), input.end());
	std::vector<unsigned char> networkAddress;
	std::vector<unsigned char> destinationAddress;
	std::cout << std::endl << "input size: " << input.size() << std::endl << std::endl;	
	ATCommand.push_back('B');
	ATCommand.push_back('H');
	//data.push_back('r');
	//data.push_back('o');
	//data.push_back('e');
	//data.push_back('l');

	networkAddress.push_back(0xFF);
	networkAddress.push_back(0xFE);
	destinationAddress.push_back(0X00);	
	destinationAddress.push_back(0X13);	
	destinationAddress.push_back(0XA2);	
	destinationAddress.push_back(0X00);	
	destinationAddress.push_back(0X40);	
	destinationAddress.push_back(0x79);	
	destinationAddress.push_back(0x5B);	
	destinationAddress.push_back(0xFE);	
	//data.at(1) = 0x0B;
	std::cout << "network address length: " << networkAddress.size() << std::endl;
	con.sendPacket( APIFrames::data(data,  destinationAddress, networkAddress));
	
	std::cout << std::endl << "end" << std::endl << std::endl;
	con.closeConnection();
	return 0;
}
