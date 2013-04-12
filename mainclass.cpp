#include "mainclass.h"

MainClass::MainClass(int argc, char * argv[])
{
	int user = getuid();
	if (user != 0)
	{
		std::cerr << "root privileges needed" << std::endl;
		throw NoRoot(); 
	}
	
	socket = new Http("http://ipsum.groept.be");

	
	try
	{
		socket->ipsumInfo();
	}
	catch(HttpError)
	{
		std::cerr << "Could not connect to Ipsum" << std::endl;
		//return 1;
	}

	
	std::cout << "argc: " << argc << std::endl;
	if(argc != 2)
	{
		std::cerr << "also provide the port number" << std::endl;
		//return 1;
	}
	db = new Sql("./sqlite/zigbee.dbs");
	con = new Connection(); 
	int connectionDescriptor = con->openPort(atoi(argv[1]), 9600);

	wsQueue = new PacketQueue();
	zbReceiveQueue = new PacketQueue();
	localZBReceiveQueue = new std::queue<Packet *>;
	localWSQueue = new std::queue<Packet *>;

	mainConditionVariable = new std::condition_variable;
	conditionVariableMutex = new std::mutex;

	zbSenderQueue = new PacketQueue();
	zbSenderConditionVariableMutex = new std::mutex;
	zbSenderConditionVariable = new std::condition_variable;

	zbSender = new ZBSender(connectionDescriptor, zbSenderConditionVariableMutex, zbSenderConditionVariable, zbSenderQueue);
	zbSenderThread = new boost::thread(boost::ref(*zbSender));

	zbReceiver = new ZBReceiver(connectionDescriptor, conditionVariableMutex, mainConditionVariable, zbReceiveQueue);
	zbReceiverThread = new boost::thread(boost::ref(*zbReceiver));

	webService = new Webservice (wsQueue, mainConditionVariable, conditionVariableMutex);

	ipsumSendQueue = new PacketQueue();
	ipsumReceiveQueue = new PacketQueue();
	ipsumConditionVariable = new std::condition_variable;
	ipsumConditionVariableMutex = new std::mutex;
	localIpsumSendQueue = new std::queue<Packet *>;
	localIpsumReceiveQueue = new std::queue<Packet *>;

	ipsum = new Ipsum(ipsumSendQueue, ipsumReceiveQueue, conditionVariableMutex, mainConditionVariable, ipsumConditionVariableMutex, ipsumConditionVariable);

	packetsWaitingForResponse = new std::queue<std::pair<Packet*, time_t>>;
}

MainClass::~MainClass()
{
	delete socket;
	delete con;
	delete db;

	delete mainConditionVariable;
	delete conditionVariableMutex;

	delete zbSenderQueue;
	delete zbSenderConditionVariableMutex;
	delete zbSenderConditionVariable;
	delete zbSender;
	delete zbSenderThread;

	delete zbReceiveQueue;
	delete localZBReceiveQueue;
	delete zbReceiver;


	
	delete wsQueue;
	delete localWSQueue;
	delete webService;

	delete ipsumSendQueue;
	delete ipsumReceiveQueue;
	delete ipsumConditionVariable;
	delete ipsumConditionVariableMutex;
	delete localIpsumSendQueue;
	delete localIpsumReceiveQueue;
	delete ipsum;
//	delete wsThread;
	delete packetsWaitingForResponse; 
}

void MainClass::operator() ()
{
	std::cout << "going into main while loop" << std::endl;
	while(true)
	{
		{	// Scope of unique_lock
			std::unique_lock<std::mutex> uniqueLock(*conditionVariableMutex);
			mainConditionVariable->wait(uniqueLock, [this]{ return ((!zbReceiveQueue->empty()) || (!wsQueue->empty() || (!ipsumReceiveQueue->empty()))); });
			std::cout << "mainconditionvariable notification received" << std::endl;
			while(!zbReceiveQueue->empty())
			{
				localZBReceiveQueue->push(zbReceiveQueue->getPacket());
				std::cout << "adding ZBPacket to local ZBReceiverQueue" << std::endl;
			}
			
			while(!wsQueue->empty())
			{
				std::cout << "type of ws packet from wsQueue: " << typeid(wsQueue->getPacket()).name() << std::endl;
				std::cout << "adding WSPacket to local WSQueue" << std::endl;
				localWSQueue->push(wsQueue->getPacket());
			}
			
			while(!ipsumReceiveQueue->empty())
			{
				localIpsumReceiveQueue->push(ipsumReceiveQueue->getPacket());
				std::cout << "adding IpsumPacket to local IpsumReceiveQueue" << std::endl;
			}
		}
		// Shared queue is no longer locked, now ready to process the packets
		Packet * packet;
		while(!localZBReceiveQueue->empty())
		{
			packet = localZBReceiveQueue->front();
			localZBReceiveQueue->pop();
			std::cout << "popped ZBPacket from local ZBQueue, type:" << typeid(packet).name() << std::endl;
			if(packet->getPacketType() == ZB_LIBEL_IO)
			{
				std::cout << "ZB_LIBEL_IO received in main" << std::endl;
				libelIOHandler(packet);
			}
			else if (packet->getPacketType() == ZB_LIBEL_MASK_RESPONSE)
			{
				std::cout << "ZB_LIBEL_MASK_RESPONSE received in main" << std::endl;
				libelMaskResponseHandler(packet);
			}
			else if (packet->getPacketType() == ZB_LIBEL_CHANGE_FREQ_RESPONSE)
			{
				std::cout << "ZB_LIBEL_CHANGE_FREQ_RESPONSE received in main" << std::endl;
				libelChangeFreqResponseHandler(packet);
			}
			else if (packet->getPacketType() == ZB_LIBEL_CHANGE_NODE_FREQ_RESPONSE)
			{
				std::cout << "ZB_LIBEL_CHANGE_NODE_FREQ_RESPONSE received in main" << std::endl;
				libelChangeNodeFreqResponseHandler(packet);
			}
			else if (packet->getPacketType() == ZB_LIBEL_ADD_NODE_RESPONSE)
			{
				std::cout << "ZB_LIBEL_ADD_NODE_RESPONSE received in main" << std::endl;
				libelAddNodeResponseHandler(packet);
			}
	  	}
		
		while(!localWSQueue->empty())
		{
			packet = localWSQueue->front();
			localWSQueue->pop();
			std::cout << "popped WSPacket from local WSQueue, type:" << typeid(packet).name() << std::endl;
			if(packet->getPacketType() == WS_COMMAND)
			{
				std::cout << "WS_PACKET received in main" << std::endl;
				webserviceHandler(packet);	
			}
		}

		while(!localIpsumReceiveQueue->empty())
		{
			packet = localIpsumReceiveQueue->front();
			localIpsumReceiveQueue->pop();
			/*
			if(typeid(packet) ==  typeid(IpsumPacket *))
			{
				std::cout << "Ipsum_PACKET received in main" << std::endl;
				
				std::cout << "post data: "  << (dynamic_cast<WSPacket *> (packet))->getRequestData() << std::endl;
			}
			*/
		}


	}
	zbReceiverThread->join();
}

void MainClass::libelIOHandler(Packet * packet)
{
	LibelIOPacket * libelIOPacket = dynamic_cast<LibelIOPacket *> (packet);
	std::cout << "temperature: " << libelIOPacket->getTemperature() << std::endl;

	std::vector<unsigned char> zigbee64BitAddress = libelIOPacket->getZigbee64BitAddress();
	std::string zigbee64BitAddressString( zigbee64BitAddress.begin(), zigbee64BitAddress.end());
	int nodeID = db->getNodeID(zigbee64BitAddressString);
	int installationID = db->getInstallationID(zigbee64BitAddressString);
	std::map<SensorType, int> availableSensors = db->getSensorsFromNode(nodeID);

	std::map<SensorType, float> sensorData = libelIOPacket->getSensorData();

	std::vector<std::tuple<SensorType, int, float>> data;
	
	for(auto it = sensorData.begin(); it != sensorData.end(); ++it)
	{
		auto sensorField = availableSensors.find(it->first);

		data.push_back(std::tuple<SensorType, int, float>(it->first, sensorField->second, it->second ));	
	}

	delete packet;

	
	//IpsumUploadPacket * ipsumUploadPacket = new IpsumUploadPacket(installationID, nodeID, data);
	//ipsumSendQueue->addPacket(dynamic_cast<Packet*> (ipsumUploadPacket));
}

void MainClass::libelMaskResponseHandler(Packet * packet)
{
	LibelMaskResponse * libelMaskResponse = dynamic_cast<LibelMaskResponse *> (packet);

	delete packet;
}

	
void MainClass::libelChangeFreqResponseHandler(Packet * packet)
{
	LibelChangeFreqResponse * libelChangeFreqResponse = dynamic_cast<LibelChangeFreqResponse *> (packet);

	delete packet;
}

void MainClass::libelChangeNodeFreqResponseHandler(Packet * packet)
{
	LibelChangeNodeFreqResponse * libelChangeNodeFreqResponse = dynamic_cast<LibelChangeNodeFreqResponse *> (packet);

	delete packet;
}

void MainClass::libelAddNodeResponseHandler(Packet * packet)
{
	LibelAddNodeResponse * libelAddNodeResponse = dynamic_cast<LibelAddNodeResponse *> (packet);

	delete packet;
}


void MainClass::webserviceHandler(Packet * packet)
{
	WSPacket * wsPacket = dynamic_cast<WSPacket *> (packet);
	switch(wsPacket->getRequestType())
	{
		case CHANGE_FREQUENCY:
			std::cout << "CHANGE_FREQUENCY request being handled" << std::endl;
			changeFrequencyHandler(wsPacket);
			break;
		case ADD_NODE:
			break;
		case ADD_SENSOR:
		     	break;
		case REQUEST_DATA:
			requestIOHandler(wsPacket);
		     	break;
		default:
		     std::cerr << "unrecognized packet" << std::endl;

	}
	delete wsPacket;
}

void MainClass::requestIOHandler(WSPacket * wsPacket)
{
		

}

void MainClass::changeFrequencyHandler(WSPacket * wsPacket)
{
	wsPacket->getRequestData();
	//findFieldInXML("sensor", "frequency");

	//LibelChangeNodeFreqPacket libelChangeNodeFreqPacket(destination64bitAddress, newFrequency):
	//zbSenderQueue->addPacket();	

}

void MainClass::addNodeHandler(WSPacket * wsPacket)
{
	char * temp;
	xercesc::DOMDocument * doc = parseToDom(wsPacket->getRequestData());

	std::string installationID, sensorGroupID, zigbeeAddress;
	
	xercesc::DOMElement * docElement = doc->getDocumentElement();	
	xercesc::DOMElement * nextElement;
	nextElement = docElement->getFirstElementChild();
	
	XMLCh * installationIDString = xercesc::XMLString::transcode("installationID");
	XMLCh * sensorGroupIDString = xercesc::XMLString::transcode("sensorGroupID");
	XMLCh * zigbeeAddressString = xercesc::XMLString::transcode("zigbeeAddress");
	while(nextElement != NULL)
	{
		if(xercesc::XMLString::compareIString(nextElement->getTagName(), installationIDString) == 0)
		{
			temp = xercesc::XMLString::transcode(nextElement->getTextContent());
			installationID = std::string(temp);
			xercesc::XMLString::release(&temp);

		}
		if(xercesc::XMLString::compareIString(nextElement->getTagName(), sensorGroupIDString) == 0)
		{
			temp = xercesc::XMLString::transcode(nextElement->getTextContent());
			sensorGroupID = std::string(temp);
			xercesc::XMLString::release(&temp);

		}
		else if(xercesc::XMLString::compareIString(nextElement->getTagName(), zigbeeAddressString) == 0)
		{
			temp = xercesc::XMLString::transcode(nextElement->getTextContent());
			zigbeeAddress = std::string(temp);
			xercesc::XMLString::release(&temp);
		}
		else
		{
			std::cerr << "invalid XML" << std::endl;
		}

		nextElement = nextElement->getNextElementSibling();
	}	


	std::string makeNewNode(int installationID, int nodeID, std::string zigbee64bitAddress);
	
	xercesc::XMLString::release(&installationIDString);
	xercesc::XMLString::release(&sensorGroupIDString);
	xercesc::XMLString::release(&zigbeeAddressString);

	db->makeNewNode(boost::lexical_cast<int> (installationID), boost::lexical_cast<int> (sensorGroupID), zigbeeAddress);


}

void MainClass::addSensorHandler(WSPacket * wsPacket)
{

}

xercesc::DOMDocument * MainClass::parseToDom(std::string data)
{

	XMLCh tempStr[100];
	
	char * temp;

	xercesc::DOMImplementation* impl = xercesc::DOMImplementation::getImplementation();
	xercesc::DOMLSParser *parser = ((xercesc::DOMImplementationLS*)impl)->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	xercesc::DOMDocument *doc;
	xercesc::Wrapper4InputSource source(new xercesc::MemBufInputSource((const XMLByte*) (data.c_str()), data.size(), "100"));
	doc = parser->parse(&source);
	
	if (doc == NULL)
	{
		throw InvalidXMLError(); 
	}
	
	
//	parser->release();

	return doc;

}

