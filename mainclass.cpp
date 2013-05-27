#include "mainclass.h"


std::map<SensorType, std::string> sensorMap;

MainClass::MainClass(int argc, char * argv[], int packetExpirationTime, unsigned char numberOfRetries) throw (StartupError): packetExpirationTime(packetExpirationTime)
{
    nextFrameID = 0;
    //Config::loadConfig("configFile.txt");
    int fd = open("errors.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    dup2 (fd, STDERR_FILENO);


    socket = new Http("http://ipsum.groept.be", "a31dd4f1-9169-4475-b316-764e1e737653");

    try
    {
        socket->ipsumInfo();
    }
    catch(HttpError)
    {
        std::cerr << "Could not connect to Ipsum" << std::endl;
        throw StartupError();
    }
/*
    std::cout << "argc: " << argc << std::endl;
    if(argc != 2)
    {
        std::cerr << "also provide the port number" << std::endl;
        throw StartupError();
    }
    std::cout << "Passed startup checks" << std::endl;

*/





    exit = false;
    db = new Sql("../zigbee.dbs");

    con = new Connection();
    int connectionDescriptor = con->openPort(atoi(argv[1]), 9600);

    addNodeSentPackets = new SentPackets<LibelAddNodePacket *, LibelAddNodeResponse *>(numberOfRetries, numberOfRetries);
    changeFreqSentPackets = new SentPackets<LibelChangeFreqPacket *, LibelChangeFreqResponse *>(numberOfRetries, numberOfRetries);

    wsReceiveQueue = new PacketQueue();
    wsSendQueue = new PacketQueue();
    zbReceiveQueue = new PacketQueue();
    localZBReceiveQueue = new std::queue<Packet *>;
    localWSReceiveQueue = new std::queue<Packet *>;

    mainConditionVariable = new std::condition_variable;
    conditionVariableMutex = new std::mutex;

    zbSenderQueue = new PacketQueue();
    zbSenderConditionVariableMutex = new std::mutex;
    zbSenderConditionVariable = new std::condition_variable;

    zbSender = new ZBSender(connectionDescriptor, zbSenderConditionVariableMutex, zbSenderConditionVariable, zbSenderQueue);
    zbSenderThread = new boost::thread(boost::ref(*zbSender));

    wsConditionVariable = new std::condition_variable;
    wsConditionVariableMutex = new std::mutex;

    zbReceiver = new ZBReceiver(connectionDescriptor, conditionVariableMutex, mainConditionVariable, zbReceiveQueue, &exit);
    zbReceiverThread = new boost::thread(boost::ref(*zbReceiver));

    webService = new Webservice (wsReceiveQueue, wsSendQueue, mainConditionVariable, conditionVariableMutex, wsConditionVariable, wsConditionVariableMutex);

    ipsumSendQueue = new PacketQueue();
    ipsumReceiveQueue = new PacketQueue();
    ipsumConditionVariable = new std::condition_variable;
    ipsumConditionVariableMutex = new std::mutex;
    localIpsumSendQueue = new std::queue<Packet *>;
    localIpsumReceiveQueue = new std::queue<Packet *>;

    sentZBPackets = new std::queue<Packet *>;

    ipsum = new Ipsum("http://ipsum.groept.be", "a31dd4f1-9169-4475-b316-764e1e737653", ipsumSendQueue, ipsumReceiveQueue, conditionVariableMutex, mainConditionVariable, ipsumConditionVariableMutex, ipsumConditionVariable);
    ipsumThread = new boost::thread(boost::ref(*ipsum));

    localZBSenderQueue = new std::vector<Packet *>;

}

MainClass::~MainClass()
{
    delete socket;
    delete con;
    delete db;

    delete addNodeSentPackets;
    delete changeFreqSentPackets;

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

    delete wsSendQueue;
    delete wsReceiveQueue;
    delete localWSReceiveQueue;
    delete webService;

    delete ipsumSendQueue;
    delete ipsumReceiveQueue;
    delete ipsumConditionVariable;
    delete ipsumConditionVariableMutex;
    delete ipsumThread;
    delete localIpsumSendQueue;
    delete localIpsumReceiveQueue;
    delete sentZBPackets;
    delete ipsum;

    delete localZBSenderQueue;
//	delete wsThread;

}

void MainClass::operator() ()
{
    std::cout << "going into main while loop" << std::endl;


    std::vector<unsigned char> zigbee64BitAddress {0X00, 0X13, 0XA2, 0X00, 0X40, 0X69, 0X73, 0X7c}; //0013A20040697376
    std::vector<SensorType> sensors {BAT, TEMP, HUM};//, PRES, VANE, PLUVIO, ANEMO, LUMINOSITY, SOLAR_RAD};
    LibelAddNodePacket * libelAddNodePacket = new LibelAddNodePacket(zigbee64BitAddress, sensors);
    localZBSenderQueue->push_back(dynamic_cast<Packet *> (libelAddNodePacket));
    addNodeSentPackets->addPacket(libelAddNodePacket);

    std::vector<std::pair<SensorType, int> > newFrequencies;
    newFrequencies.push_back(std::pair<SensorType, int > (TEMP, 3));
    //newFrequencies.push_back(std::pair<SensorType, int > (HUM, 2));

    LibelChangeFreqPacket * libelChangeFreqPacket = new LibelChangeFreqPacket(zigbee64BitAddress, newFrequencies, 1);
    //localZBSenderQueue->push_back(dynamic_cast<Packet *> (libelChangeFreqPacket));
    //changeFreqSentPackets->addPacket(libelChangeFreqPacket);

    LibelChangeNodeFreqPacket * libelChangeNodeFreqPacket = new LibelChangeNodeFreqPacket(zigbee64BitAddress, 3, 1);
    //localZBSenderQueue->push_back(dynamic_cast<Packet *> (libelChangeNodeFreqPacket));

    LibelMaskRequest  * libelMaskRequest = new LibelMaskRequest(zigbee64BitAddress, 1);
    //localZBSenderQueue->push_back(dynamic_cast<Packet *> (libelMaskRequest));

    LibelRequestIOPacket * libelRequestIOPacket = new LibelRequestIOPacket(zigbee64BitAddress, sensors, 1);


    while(true)
    {
    /*
        std::string input;
        getline(std::cin, input);

        if (input.length() > 0)
        {
            std::cout << "input: " << input << std::endl;
            exit = true;
            break;
        }
    */

        checkExpiredPackets();

        {	// Scope of unique_lock
            std::unique_lock<std::mutex> uniqueLock(*conditionVariableMutex);
            mainConditionVariable->wait(uniqueLock, [this]{ return ((!zbReceiveQueue->empty()) || (!wsReceiveQueue->empty() || (!ipsumReceiveQueue->empty()))); });
            std::cout << "mainconditionvariable notification received" << std::endl;

            /*
             *  In this next part all shared queues will be empties into local queues. By doing this the lock can be unlocked faster then when handling
             *  all packets directly from the shared queues. Thus assuring the other threads don't go into a waiting state for too long.
             */
            while(!zbReceiveQueue->empty())
            {
                localZBReceiveQueue->push(zbReceiveQueue->getPacket());
                std::cout << "adding ZBPacket to local ZBReceiverQueue" << std::endl;
            }

            while(!wsReceiveQueue->empty())
            {
                localWSReceiveQueue->push(wsReceiveQueue->getPacket());
                std::cout << "adding WSPacket to local WSQueue" << std::endl;
            }

            while(!ipsumReceiveQueue->empty())  // At the moment the ipsumReceiveQueue is not used. It could be used to back up packets that could not be sent because ipsum was down. Now these packets are cached in a queue in the ipsum thread itself.
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
            if(packet->getPacketType() == ZB_LIBEL_IO)
            {
                std::cout << "ZB_LIBEL_IO received in main" << std::endl;
                if(dynamic_cast<LibelIOPacket *> (packet) != NULL)
                {
                    libelIOHandler(dynamic_cast<LibelIOPacket *> (packet));
                }
                else
                {
                    std::cerr << "dynamic cast failed on LibelIOPacket in main" << std::endl;
                }
            }
            else if (packet->getPacketType() == ZB_LIBEL_MASK_RESPONSE)
            {
                std::cout << "ZB_LIBEL_MASK_RESPONSE received in main" << std::endl;
                if(dynamic_cast<LibelMaskResponse *> (packet) != NULL)
                {
                    libelMaskResponseHandler(dynamic_cast<LibelMaskResponse *> (packet));
                }
                else
                {
                    std::cerr << "dynamic cast failed on LibelMaskResponse in main" << std::endl;
                }
            }
            else if (packet->getPacketType() == ZB_LIBEL_CHANGE_FREQ_RESPONSE)
            {
                std::cout << "ZB_LIBEL_CHANGE_FREQ_RESPONSE received in main" << std::endl;
                if(dynamic_cast<LibelMaskResponse *> (packet) != NULL)
                {
                    libelChangeFreqResponseHandler(dynamic_cast<LibelChangeFreqResponse *> (packet));
                }
                else
                {
                    std::cerr << "dynamic cast failed on LibelChangeFreqResponse in main" << std::endl;
                }
            }
            else if (packet->getPacketType() == ZB_LIBEL_CHANGE_NODE_FREQ_RESPONSE)
            {
                std::cout << "ZB_LIBEL_CHANGE_NODE_FREQ_RESPONSE received in main" << std::endl;
                if(dynamic_cast<LibelChangeNodeFreqResponse *> (packet) != NULL)
                {
                    libelChangeNodeFreqResponseHandler(dynamic_cast<LibelChangeNodeFreqResponse *> (packet));
                }
                else
                {
                    std::cerr << "dynamic cast failed on LibelChangeNodeFreqResponse in main" << std::endl;
                }
            }
            else if (packet->getPacketType() == ZB_LIBEL_ADD_NODE_RESPONSE)
            {
                std::cout << "ZB_LIBEL_ADD_NODE_RESPONSE received in main" << std::endl;
                if(dynamic_cast<LibelAddNodeResponse *> (packet) != NULL)
                {
                    libelAddNodeResponseHandler(dynamic_cast<LibelAddNodeResponse *> (packet));
                }
                else
                {
                    std::cerr << "dynamic cast failed on LibelAddNodeResponse in main" << std::endl;
                }

            }
            else if (packet->getPacketType() == ZB_TRANSMIT_STATUS)
            {
                std::cout << "ZB_TRANSMIT_STATUS received in main" << std::endl;
                if(dynamic_cast<TransmitStatusPacket *> (packet) != NULL)
                {
                    transmitStatusHandler(dynamic_cast<TransmitStatusPacket *> (packet));
                }
                else
                {
                    std::cerr << "dynamic cast failed on TransmitStatusPacket in main" << std::endl;
                }

            }
        }

        while(!localWSReceiveQueue->empty())
        {
            packet = localWSReceiveQueue->front();
            localWSReceiveQueue->pop();
            switch (packet->getPacketType())
            {
                case WS_CHANGE_FREQUENCY_COMMAND:
                    std::cout << "WS_CHANGE_FREQUENCY_COMMAND request being handled" << std::endl;
                    changeFrequencyHandler(dynamic_cast<WSChangeFrequencyPacket*> (packet));
                    break;
                case WS_ADD_NODE_COMMAND:
                    std::cout << "WS_ADD_NODE_COMMAND request being handled" << std::endl;
                    addNodeHandler(dynamic_cast<WSAddNodePacket *> (packet));
                    break;
                case WS_ADD_SENSORS_COMMAND:
                    std::cout << "WS_ADD_SENSORS_COMMAND request being handled" << std::endl;
                    addSensorHandler(dynamic_cast<WSAddSensorsPacket*> (packet));
                break;
                case WS_REQUEST_DATA_COMMAND:
                    std::cout << "WS_REQUEST_DATA_COMMAND request being handled" << std::endl;
                    requestDataHandler(dynamic_cast<WSRequestDataPacket*> (packet));
                break;
                default:
                     std::cerr << "unrecognized packet in localWSReceiveQueue" << std::endl;
            }
        }

        while(!localIpsumReceiveQueue->empty())
        {
            packet = localIpsumReceiveQueue->front();
            localIpsumReceiveQueue->pop();
        }
    }

    zbReceiverThread->join();
    zbSenderThread->join();
    ipsumThread->join();

}

void MainClass::checkExpiredPackets()
{
    std::vector<LibelAddNodePacket *> expiredAddNodePackets = addNodeSentPackets->findExpiredPacket(localZBSenderQueue);
    for( auto it = expiredAddNodePackets.begin(); it < expiredAddNodePackets.end(); ++it )
    {
        std::cerr << "LibelAddNodePacket received no reply. " << (*it) <<  std::endl;
    }

    std::vector<LibelChangeFreqPacket *> expiredChangeFrequencyPackets = changeFreqSentPackets->findExpiredPacket(localZBSenderQueue);
    for( auto it = expiredChangeFrequencyPackets.begin(); it < expiredChangeFrequencyPackets.end(); ++it )
    {
        std::cerr << "LibelChangeFreqPacket received no reply. " << (*it) <<  std::endl;
    }
}

std::string MainClass::ucharVectToString(const std::vector<unsigned char>& ucharVect)
{
    std::stringstream stream;
    for(auto it = ucharVect.begin(); it < ucharVect.end(); ++it)
    {
        stream << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it);
    }
    return stream.str();
}

unsigned char MainClass::getNextFrameID()
{
    nextFrameID++;
    if(nextFrameID == 0) // If nextFrameID overflows it can not be 0, because 0 means : do not send an ack to this packet. We always want an ack.
        nextFrameID++;
    return nextFrameID;
}


void MainClass::libelIOHandler(LibelIOPacket * libelIOPacket)
{
    if(libelIOPacket == nullptr)
    {
        std::cerr << "dynamic cast failed on LibelIOPacket in main" << std::endl;
        return;
    }
    std::vector<unsigned char> zigbee64BitAddress = libelIOPacket->getZigbee64BitAddress();
    std::cout << "Amount of packets in localZBSenderQueue before erase: " <<  localZBSenderQueue->size() << std::endl;
    localZBSenderQueue->erase(std::remove_if(localZBSenderQueue->begin(), localZBSenderQueue->end(), [&zigbee64BitAddress, this](Packet * packet) {
            TransmitRequestPacket * zbPacket = dynamic_cast<TransmitRequestPacket *>(packet);
            if(zbPacket == NULL)
            {
                std::cerr << "localZBSenderQueue had invalid packet type | type != ZBPacket (MainClass)" << std::endl;
                return true;
            }
            else
            {
                std::vector<unsigned char> packetAddress = zbPacket->getZigbee64BitAddress();
                std::cout << "packetaddress:" << std::endl;
                for(auto it = packetAddress.begin(); it < packetAddress.end(); ++it)
                {
                    std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it) << " ";
                }
                std::cout << std::endl << "zigbee64BitAddress:" << std::endl;
                for(auto it = zigbee64BitAddress.begin(); it < zigbee64BitAddress.end(); ++it)
                {
                    std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it) << " ";
                }

                if(std::equal(packetAddress.begin(), packetAddress.end(), zigbee64BitAddress.begin()))
                {
                    std::cout << "packet added to zbSenderQueue from localZBSenderQueue" << std::endl;
                    zbSenderQueue->addPacket(dynamic_cast<Packet *> (zbPacket));
                    return true;
                }
                else
                {
                    return false;
                }
            }}), localZBSenderQueue->end());
    std::cout << "Amount of packets left in localZBSenderQueue after erase: " <<  localZBSenderQueue->size() << std::endl;
    {
        std::lock_guard<std::mutex> lgSender(*zbSenderConditionVariableMutex);
        zbSenderConditionVariable->notify_all();
        std::cout << "zbsender notified from libeliohandler" << std::endl;
    }

    std::string zigbee64BitAddressString(ucharVectToString(zigbee64BitAddress));
    int nodeID, installationID;
    std::map<SensorType, int> availableSensors;
    std::cout << "zigbee64BitAddressString: " << zigbee64BitAddressString << std::endl;
    try
    {
        std::cout << "before getNodeID" << std::endl;
        nodeID = db->getNodeID(zigbee64BitAddressString);
        std::cout << "before getInstallationID" << std::endl;
        installationID = db->getInstallationID(zigbee64BitAddressString);
        std::cout << "before getSensorsFromNode" << std::endl;
        availableSensors = db->getSensorsFromNode(nodeID);
    }
    catch (SqlError)
    {
        std::cerr << "Could not upload data since this sensor was not known to the sql db" << std::endl;
        return;
    }
    std::map<SensorType, float> sensorData = libelIOPacket->getSensorData();

    std::vector<std::tuple<SensorType, int, float>> data;

    for(auto it = sensorData.begin(); it != sensorData.end(); ++it)
    {
        auto sensorField = availableSensors.find(it->first);
        if(sensorField == availableSensors.end())
        {
            std::cerr << "Data received from sensor which could not be found in database"  << std::endl;
        }
        else
        {
            std::cout << "sensor data that will be uploaded: " << it->first << std::endl;
            data.push_back(std::tuple<SensorType, int, float>(it->first, sensorField->second, it->second ));
        }

    }

    delete libelIOPacket;


    IpsumUploadPacket * ipsumUploadPacket = new IpsumUploadPacket(installationID, nodeID, data);

    std::vector<std::tuple<SensorType, int, float> > retreivedData = ipsumUploadPacket->getData();
    for(auto it = retreivedData.begin(); it < retreivedData.end(); ++it)
    {
        std::cout << "sensortypes in retreived data: " << std::get<0>(*it) << std::endl;
    }

    ipsumSendQueue->addPacket(dynamic_cast<Packet*> (ipsumUploadPacket));
    std::cout << "ipsumuploadpacket added" << std::endl;
    std::lock_guard<std::mutex> lg(*ipsumConditionVariableMutex);
    ipsumConditionVariable->notify_all();
}

void MainClass::libelMaskResponseHandler(LibelMaskResponse * libelMaskResponse)
{
    // Mask requests are never sent so responses are not handled yet. The mask of a sensor can be found by checking ipsum on the in use parameter of every sensor.
    if(libelMaskResponse == nullptr)
    {
        std::cerr << "dynamic cast failed on LibelMaskResponse in main" << std::endl;
        return;
    }
    delete libelMaskResponse;
}


void MainClass::libelChangeFreqResponseHandler(LibelChangeFreqResponse * libelChangeFreqResponse)
{
    if(libelChangeFreqResponse == nullptr)
    {
        std::cerr << "dynamic cast failed on LibelChangeFreqResponse in main" << std::endl;
        return;
    }

    LibelChangeFreqPacket * libelChangeFreqPacket = changeFreqSentPackets->retrieveCorrespondingPacket(libelChangeFreqResponse);

    if(libelChangeFreqPacket != nullptr)
    {
        changeFreqSentPackets->removePacket(libelChangeFreqPacket);
    }

    int installationID = -1;
    int sensorGroupID = -1;
    std::map<SensorType, int> sensors;

    try
    {
    db->getInstallationID(ucharVectToString(libelChangeFreqResponse->getZigbee64BitAddress()));
    db->getNodeID(ucharVectToString(libelChangeFreqResponse->getZigbee64BitAddress()));
    sensors = db->getSensorsFromNode(sensorGroupID); // sensors is a vector of sensorType + ipsum ID
    }
    catch(SqlError e)
    {
        std::cout << e.what() << std::endl;
        return;
    }

    std::map<SensorType, int> frequencies = libelChangeFreqResponse->getFrequencies(); // frequencies is a map of sensortype + frequency(interval)
    std::vector<std::pair<int, int> > ipsumFreqVector;
    for(auto it = sensors.begin(); it != sensors.end(); ++it)
    {
        auto found = frequencies.find(it->first);
        if (found != frequencies.end())
        {
            ipsumFreqVector.push_back(std::pair<int, int> (it->second, found->second));      // adding ipsumId and frequency to the output vector
        }
        else
        {
            std::cerr << "MainClass: A sensor frequency has been changed for an unknown sensor" << std::endl;
        }
    }
    IpsumChangeFreqPacket * ipsumChangeFreqPacket = new IpsumChangeFreqPacket(installationID, sensorGroupID, ipsumFreqVector);

    ipsumSendQueue->addPacket(ipsumChangeFreqPacket);
    std::lock_guard<std::mutex> lg(*ipsumConditionVariableMutex);
    ipsumConditionVariable->notify_all();
    std::cout << "ChangeFreqResponse handled and ipsum packet created" << std::endl;

    std::cout << "exiting libelChangeFreqResponseHandler" << std::endl;

    delete libelChangeFreqResponse;
}

void MainClass::libelChangeNodeFreqResponseHandler(LibelChangeNodeFreqResponse * libelChangeNodeFreqResponse)
{
    // Node frequency packets should not be sent so responses on these packets are not handled. Frequencies are changed sensor per sensor and not for 1 node at once.

    if(libelChangeNodeFreqResponse == nullptr)
    {
        std::cerr << "dynamic cast failed on LibelChangeNodeFreqResponse in main" << std::endl;
        return;
    }


    delete libelChangeNodeFreqResponse;
}

void MainClass::libelAddNodeResponseHandler(LibelAddNodeResponse * libelAddNodeResponse)
{
    std::cout << "MainClass::libelAddNodeResponseHandler(LibelAddNodeResponse * libelAddNodeResponse)" << std::endl;

    if(libelAddNodeResponse == nullptr)
    {
        std::cerr << "dynamic cast failed on LibelAddNodeResponse in main" << std::endl;
        return;
    }

    LibelAddNodePacket * libelAddNodePacket = addNodeSentPackets->retrieveCorrespondingPacket(libelAddNodeResponse);

    if(libelAddNodePacket != nullptr)
    {
        addNodeSentPackets->removePacket(libelAddNodePacket);
        std::cout << "removed libelAddNodePacket from sentQueue" << std::endl;
    }


    int installationID = -1;
    int sensorGroupID = -1;
    std::map<SensorType, int> sensorsFromDB ;
    try
    {
        installationID = db->getInstallationID(ucharVectToString(libelAddNodeResponse->getZigbee64BitAddress()));
        sensorGroupID = db->getNodeID(ucharVectToString(libelAddNodeResponse->getZigbee64BitAddress()));
        sensorsFromDB = db->getSensorsFromNode(sensorGroupID);
    }
    catch (SqlError)
    {
        std::cerr << "Could not upload data since this sensor was not known to the sql db" << std::endl;
        return;
    }

    std::vector<SensorType> sensorsFromPacket = libelAddNodeResponse->getSensors();
    std::map<int, bool> sensorIDs;       // Sensor IDs needed for the ipsum packet to change the inuse of that sensor to either true or false.

    bool sensorFound = false;
    for(auto sensorsFromDBIt = sensorsFromDB.begin(); sensorsFromDBIt != sensorsFromDB.end(); ++sensorsFromDBIt)
    {
        for(auto sensorsFromPacketIt = sensorsFromPacket.begin(); sensorsFromPacketIt < sensorsFromPacket.end(); ++sensorsFromPacketIt)
        {
            if ((*sensorsFromPacketIt) == (sensorsFromDBIt->first))
            {
                sensorFound = true;
            }
        }
        sensorIDs.insert(std::pair<int, bool> (sensorsFromDBIt->second, sensorFound));
        sensorFound = false;
    }

    #ifdef DEBUG
    std::cout << "\033[1;31m sensors to change in use. \033[0m" << std::endl;
    for(auto it = sensorIDs.begin(); it != sensorIDs.end(); ++it)
    {
        std::cout << "sensor found: " << (int) it->first << " bool " << it->second << std::endl;
    }
    #endif

    IpsumChangeInUsePacket * ipsumChangeInUsePacket = new IpsumChangeInUsePacket(installationID, sensorGroupID, sensorIDs, true);
    std::cout << "adding ipsumpacket" << std::endl;
    ipsumSendQueue->addPacket(ipsumChangeInUsePacket);
    std::lock_guard<std::mutex> lg(*ipsumConditionVariableMutex);
    ipsumConditionVariable->notify_all();
    std::cout << "AddNodeResponse handled and ipsum packet created" << std::endl;

    std::cout << "exiting libelAddNodeResponseHandler" << std::endl;

    delete libelAddNodeResponse;
}

void MainClass::transmitStatusHandler(TransmitStatusPacket * transmitStatusPacket)
{
    std::cout << "MainClass::transmitStatusHandler(TransmitStatusPacket * transmitStatusPacket) " << std::endl;
    if(transmitStatusPacket == nullptr)
    {
        std::cerr << "dynamic cast failed on TransmitStatusPacket in main" << std::endl;
        return;
    }
    if(transmitStatusPacket->getDeliveryStatus() != 0)          // else packet was delivered succesfully to the zigbee radio it was intended for. But we can not be sure that the waspmote received it correctly. For this we wait for the response packet from libelium.
    {
        // Find a packet in one of the sent packet queues that has a corresponding frame ID and if so check if sending failed.
        // If sending failed then send it again.
        LibelAddNodePacket * addNodePacket = addNodeSentPackets->findResendablePacket(transmitStatusPacket->getFrameID());
        LibelChangeFreqPacket * changeFreqPacket = changeFreqSentPackets->findResendablePacket(transmitStatusPacket->getFrameID());
        if(addNodePacket != nullptr && changeFreqPacket  != nullptr)
        {
            if(addNodePacket->getTimeOfLastSending() > changeFreqPacket->getTimeOfLastSending())
            {
                std::cout << "resending addNodePacket" << std::endl;
                addNodePacket->setTimeOfLastSending(0);
                //localZBSenderQueue->push_back(addNodePacket.first);
            }
            else
            {
                std::cout << "resending changeFreqPacket" << std::endl;
                changeFreqPacket->setTimeOfLastSending(0);
                //localZBSenderQueue->push_back(changeFreqPacket.first);
            }
        }
        else if(addNodePacket  != nullptr)
        {
            addNodePacket->setTimeOfLastSending(0);
            std::cout << "resending addNodePacket" << std::endl;
            //localZBSenderQueue->push_back(addNodePacket.first);
        }
        else if(changeFreqPacket  != nullptr)
        {
            std::cout << "resending changeFreqPacket" << std::endl;
            changeFreqPacket->setTimeOfLastSending(0);
            //localZBSenderQueue->push_back(changeFreqPacket.first);
        }
    }
    delete transmitStatusPacket;
}

void MainClass::requestDataHandler(WSRequestDataPacket * wsRequestDataPacket)
{
    std::cout << "MainClass::requestDataHandler(WSRequestDataPacket * wsRequestDataPacket)" << std::endl;
    if(wsRequestDataPacket == nullptr)
    {
        std::cerr << "dynamic cast failed on WSRequestDataPacket in main" << std::endl;
        return;
    }

    std::string zigbee64BitAddress = db->getNodeAddress(wsRequestDataPacket->getSensorGroupID());
    std::map<SensorType,int> sensorsFromDB = db->getSensorsFromNode(wsRequestDataPacket->getSensorGroupID());

    std::vector<int> sensors = wsRequestDataPacket->getSensors();
    std::vector<SensorType> sensorTypes;    // vector of SensorTypes used to construct a LibelRequestIOPacket
    for(auto sensorsIt = sensors.begin(); sensorsIt < sensors.end(); ++sensorsIt)
    {
        for(auto sensorsFromDBIt = sensorsFromDB.begin(); sensorsFromDBIt != sensorsFromDB.end(); ++sensorsFromDBIt)
        {
            if(sensorsFromDBIt->second == (*sensorsIt))
            {
                sensorTypes.push_back(sensorsFromDBIt->first);  // interval / 10 since ZB works with 10s as a unit of time
            }
        }

    }

    LibelRequestIOPacket * libelRequestIOPacket = new LibelRequestIOPacket(std::vector<unsigned char>(zigbee64BitAddress.begin(), zigbee64BitAddress.end()), sensorTypes, getNextFrameID());

    localZBSenderQueue->push_back(dynamic_cast<Packet *> (libelRequestIOPacket));
    //zbSenderQueue->addPacket(libelRequestIOPacket);
    //std::lock_guard<std::mutex> lg(*zbSenderConditionVariableMutex);
    //zbSenderConditionVariable->notify_all();
    delete wsRequestDataPacket;
}

void MainClass::changeFrequencyHandler(WSChangeFrequencyPacket *  wsChangeFrequencyPacket)
{
    std::cout << "MainClass::changeFrequencyHandler(WSChangeFrequencyPacket *  wsChangeFrequencyPacket)" << std::endl;
    if(wsChangeFrequencyPacket == nullptr)
    {
        std::cerr << "dynamic cast failed on WSChangeFrequencyPacket in main" << std::endl;
        return;
    }


    std::cout << "changeFrequencyHandler(WSChangeFrequencyPacket *  wsChangeFrequencyPacket)" << std::endl;

    std::string zigbee64BitAddress = db->getNodeAddress(wsChangeFrequencyPacket->getSensorGroupID());
    std::map<SensorType,int> sensors = db->getSensorsFromNode(wsChangeFrequencyPacket->getSensorGroupID());

    std::vector<std::pair<SensorType, int> > newFrequencies;    // SensorType + frequency(interval) in seconds
    std::vector<std::pair<int, int> > frequencies = wsChangeFrequencyPacket->getFrequencies();
    for(auto it = frequencies.begin(); it < frequencies.end(); ++it)
    {
        std::cout << "frequencies.first: " << it->first << std::endl;
        for(auto sensorsIt = sensors.begin(); sensorsIt != sensors.end(); ++sensorsIt)
        {
            std::cout << "sensors.second: " << sensorsIt->first << std::endl;
            if(sensorsIt->second == it->first)
            {
                newFrequencies.push_back(std::pair<SensorType, int>(sensorsIt->first, it->second/10));  // interval / 10 since ZB works with 10s as a unit of time
            }
        }

    }

    LibelChangeFreqPacket * libelChangeFreqPacket = new LibelChangeFreqPacket(convertStringToVector(zigbee64BitAddress), newFrequencies, getNextFrameID());

    localZBSenderQueue->push_back(dynamic_cast<Packet *> (libelChangeFreqPacket));
    std::cout << "libelChangeFreqPacket added tot localZBSenderQueue" << std::endl;
    //zbSenderQueue->addPacket(libelChangeFreqPacket);
    //std::lock_guard<std::mutex> lg(*zbSenderConditionVariableMutex);
    //zbSenderConditionVariable->notify_all();

    changeFreqSentPackets->addPacket(libelChangeFreqPacket);
    std::cout << "end of changeFreqHandler()" << std::endl;
    delete wsChangeFrequencyPacket;
}

void MainClass::addNodeHandler(WSAddNodePacket *wsAddNodePacket)
{
    if(wsAddNodePacket == nullptr)
    {
        std::cerr << "dynamic cast failed on WSAddNodePacket in main" << std::endl;
        return;
    }

    db->makeNewNode(wsAddNodePacket->getInstallationID(),wsAddNodePacket->getSensorGroupID(), wsAddNodePacket->getZigbeeAddress64Bit());

    delete wsAddNodePacket;
}

void MainClass::addSensorHandler(WSAddSensorsPacket *wsAddSensorsPacket)
{
    std::cout << "MainClass::addSensorHandler(WSAddSensorsPacket *wsAddSensorsPacket)" << std::endl;

    if(wsAddSensorsPacket == nullptr)
    {
        std::cerr << "dynamic cast failed on WSAddSensorsPacket in main" << std::endl;
        return;
    }

    std::map<SensorType, int> sensors = wsAddSensorsPacket->getSensors();
    int sensorGroupID = wsAddSensorsPacket->getSensorGroupID();

    // Add these sensors to the right node in the database

    for(auto it = sensors.begin(); it != sensors.end(); ++it)
    {
        db->updateSensorsInNode(sensorGroupID, it->first, it->second);
    }

    // Send a LibelAddNodePacket
    std::string zigbee64BitAddressString = db->getNodeAddress(sensorGroupID);
    std::cout << "Zigbee64BitAddress for add node packet (from string)" << std::endl << zigbee64BitAddressString << std::endl;
    std::vector <unsigned char> zigbee64BitAddress = convertStringToVector(zigbee64BitAddressString);


    std::cout << "Zigbee64BitAddress for add node packet (from vector)" << std::endl;
    for(auto it = zigbee64BitAddress.begin(); it < zigbee64BitAddress.end(); ++it)
    {
        std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex  << (int) (*it) << " ";
    }

    std::map<SensorType,int> sensorsFromDB = db->getSensorsFromNode(sensorGroupID);
    std::vector<SensorType> sensorTypes;
    for(auto it = sensorsFromDB.begin(); it != sensorsFromDB.end(); ++it)
    {
        sensorTypes.push_back(it->first);
    }

    LibelAddNodePacket * packet =  new LibelAddNodePacket(zigbee64BitAddress, sensorTypes, getNextFrameID());

    //zbSenderQueue->addPacket(dynamic_cast<Packet *> (packet));
    localZBSenderQueue->push_back(dynamic_cast<Packet *> (packet));
    addNodeSentPackets->addPacket(packet);
    std::cout << "LocalZBSenderQueue size: " << localZBSenderQueue->size() << std::endl << std::endl;

    //std::lock_guard<std::mutex> lg(*zbSenderConditionVariableMutex);
    //zbSenderConditionVariable->notify_azigbeeAddressll();
    delete wsAddSensorsPacket;
}

std::vector<unsigned char> MainClass::convertStringToVector(std::string input)
{
    std::vector<unsigned char> output;
    for(auto it = input.begin(); it != input.end(); it += 2)
    {
        std::string slice(it, it + 2);

        unsigned int x;
        std::stringstream ss;
        ss << std::hex << slice;
        ss >> x;
        // output it as a signed type

        output.push_back(x);
    }
    return output;
}
