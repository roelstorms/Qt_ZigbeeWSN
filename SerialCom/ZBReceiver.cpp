#include "ZBReceiver.h"


ZBReceiver::ZBReceiver(int connectionDescriptor, std::mutex * conditionVariableMutex, std::condition_variable * mainConditionVariable, PacketQueue * zbReceiveQueue) : connectionDescriptor(connectionDescriptor), conditionVariableMutex(conditionVariableMutex), mainConditionVariable(mainConditionVariable), zbReceiveQueue(zbReceiveQueue)
{	
	std::cout << "ZBReceiver constructor" << std::endl;
}


ZBReceiver::~ZBReceiver()
{
	std::cout << "ZBReceiver destructor" << std::endl;
}

unsigned char ZBReceiver::readByte()
{
	int input = 0x0;
    read(connectionDescriptor, &input, 1);
            /*
    while(read(fd, &input, 1) <= 0)
	{

	}
    */

	printf("%X\n", input);
	if(input == 0x7D)
	{
        while(read(connectionDescriptor, &input, 1) <= 0)
		{

		}
		std::cout << "input: " << std::hex << input << std::endl;	
		input = input ^ 0x20;
		std::cout << "XOR'ed input: " << std::hex << input << std::endl;	
	}
	return input;
	
	//return 0x0;	// Not correct since input could be 0X0 as well. A bool should be returned to let the caller know if a byte was read or not. 
			// A pointer to a buffer could be passed as argument to store the unsigned char read from fd. 
			// Need to figure out where I want to put the blocking while loop that waits for input. In this function or somewhere when this function is called.
}


void ZBReceiver::operator() ()
{
	unsigned char input = 0x0;
	std::vector<unsigned char> packetVector;	
	while(true)
    	{
        input = readByte();
		if(input == 0x7E)
		{
			packetVector.push_back(input);
			
            input = readByte();
			int packetSize = input * 255;
			packetVector.push_back(input);

            input = readByte();
			packetSize += input;
			packetVector.push_back(input);
	
			printf("pSize: %d\n", packetSize);
			for(int position = 0; position <= packetSize; ++position)
			{
                input = readByte();
				packetVector.push_back(input);
				fflush(stdout);
			}
			unsigned char packetType = packetVector.at(3);	
			std::cout << "packet type: " << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) packetType << std::endl;
			
			Packet * packet;
			switch(packetType)
			{
				case 0x90:
					switch(packetVector.at(15))	//It is a ZB data packet, all our libelium packets are of this type, so now to figure out what libelium packet we 've got
					{
						case 0x2:
                            std::cout << "found LibelAddNodeResponse" << std::endl;
							packet = dynamic_cast<Packet*> (new LibelAddNodeResponse(packetVector));
							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;
						case 0x4:
							std::cout << "found LibelMaskResponse" << std::endl;
							packet = dynamic_cast<Packet*> (new LibelMaskResponse(packetVector));
							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;
						case 0x6:
							std::cout << "found LibelChangeNodeFreqResponse" << std::endl;
							packet = dynamic_cast<Packet*> (new LibelChangeNodeFreqResponse(packetVector));
							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;
						case 0x8:
							std::cout << "found LibelChangeFreqResponse" << std::endl;
							packet = dynamic_cast<Packet*> (new LibelChangeFreqResponse(packetVector));
							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;
						case 0xA:
							std::cout << "found LibelIOpacket" << std::endl;
							packet = dynamic_cast<Packet*> (new LibelIOPacket(packetVector));
							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;

						
						default:
						throw UnknownPacketType();
				
					}
					break;

				case 0x92:
					//packet = dynamic_cast<Packet*> (new DataIOPacket(packetVector));
					break;

				case 0x88:
					std::cout << "ATCommandResponsePacket received, and thrown away" << std::endl;
						//Processing needed here
				break;
				default :
				throw UnknownPacketType();

			}

			packetVector.clear();
		//	std::cout << "analog data on pin 1: " << std::to_string(APIPacket.readAnalog(0)) << std::endl;
		}
    	}
	logFile.close();
	std::cout << "End of inputhandler" << std::endl;	
}
