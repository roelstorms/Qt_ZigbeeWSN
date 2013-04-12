#include "../ATCommandPacket.h"
#include "unistd.h"
#include "stdlib.h"

int main(int argc, char* argv[])
{
	std::vector<unsigned char> ATCommand, parameter;
	ATCommand.push_back('O');	
	ATCommand.push_back('P');	
	ATCommandPacket atcommandpacket(ATCommand, parameter, 1, false);
	std::cout << "ATCommandPacket: " << atcommandpacket << std::endl;
	return 0;
}
