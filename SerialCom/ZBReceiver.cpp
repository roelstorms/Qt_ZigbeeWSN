#include "ZBReceiver.h"


ZBReceiver::ZBReceiver(bool * stop, int connectionDescriptor, std::mutex * conditionVariableMutex, std::condition_variable * mainConditionVariable, PacketQueue * zbReceiveQueue) : stop(stop), connectionDescriptor(connectionDescriptor), conditionVariableMutex(conditionVariableMutex), mainConditionVariable(mainConditionVariable), zbReceiveQueue(zbReceiveQueue)
{	
	std::cout << "ZBReceiver constructor" << std::endl;
    logFile.open("receivedpacketlog.txt", std::ios::in | std::ios::app);
}

ZBReceiver::~ZBReceiver()
{
	std::cout << "ZBReceiver destructor" << std::endl;
    logFile.close();
}

bool ZBReceiver::readByte(unsigned char & buf)
{
    int bytesRead = read(connectionDescriptor, &buf, 1);

    while(bytesRead <= 0 )
	{
        bytesRead = read(connectionDescriptor, &buf, 1);    // Will block for 1s or when input is available
        if((*stop))
        {
            return false;
        }
    }

    printf("%X\n", buf);

    if(buf == 0x7D) // 7D is escape character
    {
        if(read(connectionDescriptor, &buf, 1) <= 0)
        {
            // After 7D another byte should always follow, read will wait 1s (see configuration in connection.cpp)
            // If it takes longer then 1s something is wrong and we return false;
            //std::cerr << "reading a byte didn't return 1 byte" << std::endl;
            return false;
        }
        std::cout << "input: " << std::hex << buf << std::endl;
        buf = buf ^ 0x20;
        std::cout << "XOR'ed input: " << std::hex << buf << std::endl;
    }
    return true;
}

void ZBReceiver::operator() ()
{
	unsigned char input = 0x0;
	std::vector<unsigned char> packetVector;	
    while(!(*stop))
    {
        packetVector.clear();
        if(!readByte(input)) continue;      // This means that stop has been set to true or that a read took unexpectidly long
                                            // So we start a new iteration of the while loop. If stop is indeed true, this thread will stop
                                            // else normal operation will continue and maybe 1 packet has been lost but this should never happen

		if(input == 0x7E)
		{
			packetVector.push_back(input);
			
            if(!readByte(input)) continue;
			int packetSize = input * 255;
			packetVector.push_back(input);

            if(!readByte(input)) continue;
			packetSize += input;
			packetVector.push_back(input);
	
			printf("pSize: %d\n", packetSize);
			for(int position = 0; position <= packetSize; ++position)
			{
                if(!readByte(input)) continue;
				packetVector.push_back(input);
				fflush(stdout);
			}
			unsigned char packetType = packetVector.at(3);	
			std::cout << "packet type: " << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) packetType << std::endl;

            boost::posix_time::ptime now = boost::posix_time::second_clock::local_time(); //use the clock

			Packet * packet;
			switch(packetType)
			{
				case 0x90:
					switch(packetVector.at(15))	//It is a ZB data packet, all our libelium packets are of this type, so now to figure out what libelium packet we 've got
					{
						case 0x2:
                            std::cout << "found LibelAddNodeResponse" << std::endl;

                            try
                            {
                                packet = dynamic_cast<Packet*> (new LibelAddNodeResponse(packetVector));
                            }
                            catch(InvalidPacketType e)
                            {
                                std::cerr << e.what() << std::endl;
                                break;
                            }

                            #ifdef PACKET_LOGGING
                                logFile << boost::posix_time::to_simple_string(now) << " : found LibelAddNodeResponse: " << dynamic_cast<ZBPacket*> (packet) << std::endl;
                            #endif

							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;
						case 0x4:
							std::cout << "found LibelMaskResponse" << std::endl;
                            try
                            {
                                packet = dynamic_cast<Packet*> (new LibelMaskResponse(packetVector));
                            }
                            catch(InvalidPacketType e)
                            {
                                std::cerr << e.what() << std::endl;
                                break;
                            }

                            #ifdef PACKET_LOGGING
                                logFile << boost::posix_time::to_simple_string(now) << " : found LibelMaskResponse: " <<  dynamic_cast<ZBPacket*> (packet)  << std::endl;
                            #endif

							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;
						case 0x6:
							std::cout << "found LibelChangeNodeFreqResponse" << std::endl;

                            try
                            {
                                packet = dynamic_cast<Packet*> (new LibelChangeNodeFreqResponse(packetVector));
                            }
                            catch(InvalidPacketType e)
                            {
                                std::cerr << e.what() << std::endl;
                                break;
                            }

                            #ifdef PACKET_LOGGING
                                logFile << boost::posix_time::to_simple_string(now) << " :  found LibelChangeNodeFreqResponse: " <<  dynamic_cast<ZBPacket*> (packet)  << std::endl;
                            #endif

							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;
						case 0x8:
							std::cout << "found LibelChangeFreqResponse" << std::endl;
                            try
                            {
                                packet = dynamic_cast<Packet*> (new LibelChangeFreqResponse(packetVector));
                            }
                            catch(InvalidPacketType e)
                            {
                                std::cerr << e.what() << std::endl;
                                break;
                            }

                            #ifdef PACKET_LOGGING
                                logFile << boost::posix_time::to_simple_string(now) << " : found LibelChangeFreqResponse: " <<  dynamic_cast<ZBPacket*> (packet)  << std::endl;
                            #endif

							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;
						case 0xA:
							std::cout << "found LibelIOpacket" << std::endl;
                            try
                            {
                                packet = dynamic_cast<Packet*> (new LibelIOPacket(packetVector));
                            }
                            catch(InvalidPacketType e)
                            {
                                std::cerr << e.what() << std::endl;
                                break;
                            }

                            #ifdef PACKET_LOGGING
                                std::cout << boost::posix_time::to_simple_string(now) << " : found LibelIOpacket: " <<  dynamic_cast<ZBPacket*> (packet)  << std::endl;
                                logFile << boost::posix_time::to_simple_string(now) << " : found LibelIOpacket: " <<  dynamic_cast<ZBPacket*> (packet)  << std::endl;
                            #endif

							zbReceiveQueue->addPacket(packet);
							{
								std::lock_guard<std::mutex> lg(*conditionVariableMutex);
								mainConditionVariable->notify_all();
								std::cout << "main notified" << std::endl;
							}
						break;

						
						default:
                        std::cerr << "unknown packet type received in ZBReceiver" << std::endl;

                        #ifdef PACKET_LOGGING
                            logFile << boost::posix_time::to_simple_string(now) << " : unknown libel packet logged: ";

                            for(auto it = packetVector.begin(); it < packetVector.end(); ++it)
                            {
                                logFile << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it) << " ";
                            }
                            logFile << std::endl;
                        #endif
					}
					break;

				case 0x92:
                    std::cerr << "DataIOPacket received, and thrown away (we use normal data packets to receive IO, not the ones defines by zigbee" << std::endl;
                    #ifdef PACKET_LOGGING
                        logFile << boost::posix_time::to_simple_string(now) << " : DataIOPacket received and thrown away (should not be used in the network)" << std::endl;
                    #endif
					break;

				case 0x88:
                    std::cerr << "ATCommandResponsePacket received, and thrown away" << std::endl;
                    #ifdef PACKET_LOGGING
                        logFile << boost::posix_time::to_simple_string(now) << " : ATCommandResponsePacket received and thrown away (should not be used in the network)" << std::endl;
                    #endif
						//Processing needed here
                break;

                case 0X8B:
                std::cout << "found TransmitStatus packet" << std::endl;
                try
                {
                    packet = dynamic_cast<Packet*> (new TransmitStatusPacket(packetVector));
                }
                catch(InvalidPacketType e)
                {
                    std::cerr << e.what() << std::endl;
                    break;
                }
                #ifdef PACKET_LOGGING
                    logFile << boost::posix_time::to_simple_string(now) << " : found TransmitStatusPacket: " <<  dynamic_cast<ZBPacket*> (packet)  << std::endl;
                #endif
                zbReceiveQueue->addPacket(packet);
                {
                    std::lock_guard<std::mutex> lg(*conditionVariableMutex);
                    mainConditionVariable->notify_all();
                    std::cout << "main notified" << std::endl;
                }
                break;

				default :
                std::cerr << "unknown packet type received in ZBReceiver" << std::endl;

                #ifdef PACKET_LOGGING
                    logFile << boost::posix_time::to_simple_string(now) << " : unknown zigbee packet logged: ";

                    for(auto it = packetVector.begin(); it < packetVector.end(); ++it)
                    {
                        logFile << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it) << " ";
                    }
                    logFile << std::endl;
                #endif

			}


		}
    }

	std::cout << "End of inputhandler" << std::endl;	
}
