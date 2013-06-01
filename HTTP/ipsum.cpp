#include "ipsum.h"

Ipsum::Ipsum(std::string ipsumURL, std::string ipsumPersonalKey, PacketQueue * ipsumSendQueue, PacketQueue * ipsumReceiveQueue, std::mutex * mainConditionVariableMutex, std::condition_variable * mainConditionVariable, std::mutex * ipsumConditionVariableMutex, std::condition_variable * ipsumConditionVariable) : ipsumSendQueue(ipsumSendQueue), ipsumReceiveQueue(ipsumReceiveQueue), mainConditionVariableMutex(mainConditionVariableMutex), ipsumConditionVariableMutex(ipsumConditionVariableMutex), mainConditionVariable(mainConditionVariable), ipsumConditionVariable(ipsumConditionVariable), IpsumUnreachable(false)
{
	std::cout << "ipsum constructor" << std::endl;
	localIpsumSendQueue = new std::queue<Packet*>;
    http = new Http(ipsumURL, ipsumPersonalKey);
}

Ipsum::~Ipsum()
{
	delete http;
	delete localIpsumSendQueue;
}

void Ipsum::operator()()
{
	std::cout << "Ipsum::operator()()" << std::endl;
    while(true)
    {
        std::unique_lock<std::mutex> uniqueLock(*ipsumConditionVariableMutex);
        ipsumConditionVariable->wait(uniqueLock, [this] {return (!ipsumSendQueue->empty());});

        std::cout << "ipsum condition variable notified" << std::endl;

        while(!ipsumSendQueue->empty())
        {
            localIpsumSendQueue->push(ipsumSendQueue->getPacket());
        }

        if (IpsumUnreachable)
        {
            try
            {
                http->ipsumInfo();
                IpsumUnreachable = false;
            }
            catch (HttpError)
            {
                IpsumUnreachable = true;
            }
        }

        if(!IpsumUnreachable)
        {
            Packet * ipsumPacket;
            while(!localIpsumSendQueue->empty())
            {
                ipsumPacket = localIpsumSendQueue->front();

                switch(ipsumPacket->getPacketType())
                {
                    case IPSUM_UPLOAD:
                        uploadDataHandler(dynamic_cast<IpsumUploadPacket *> (ipsumPacket));
                    break;
                    case IPSUM_CHANGE_IN_USE:
                        changeInUseHandler(dynamic_cast<IpsumChangeInUsePacket *> (ipsumPacket));
                    break;
                    case IPSUM_CHANGE_FREQ:
                        changeFrequencyHandler(dynamic_cast<IpsumChangeFreqPacket *> (ipsumPacket));
                    break;
                    default:
                    std::cerr << "packet type not recognized in ipsum" << std::endl;
                    // Packet not recognized

                }
            }
        }
        else
        {
            std::cerr << "Ipsum unreachable, packed stored in queue" << std::endl;
        }
    }
}


void Ipsum::uploadDataHandler(IpsumUploadPacket * packet)
{
    #ifdef IPSUM_DEBUG
    std::cout << "Ipsum::uploadDataHandler(IpsumUploadPacket * packet)" << std::endl;
    #endif

    try
    {
        http->uploadData(packet);
        localIpsumSendQueue->pop();
        delete packet;
    }
    catch(HttpError)
    {
        std::cerr << "Error trying to upload data to ipsum" << std::endl;
        try
        {
            http->ipsumInfo();

        }
        catch (HttpError)
        {
            IpsumUnreachable = true;
        }
    }
}

void Ipsum::changeInUseHandler(IpsumChangeInUsePacket * packet)
{
    #ifdef IPSUM_DEBUG
    std::cout << "Ipsum::changeInUseHandler(IpsumChangeInUsePacket * packet)" << std::endl;
    #endif
    try
    {
        http->changeInUse(packet);
        localIpsumSendQueue->pop();
        delete packet;
    }
    catch(HttpError)
    {
        std::cerr << "Error trying to changing inuse on ipsum" << std::endl;
        try
        {
            http->ipsumInfo();
        }
        catch (HttpError)
        {
            IpsumUnreachable = true;
        }
    }
    catch(InvalidXMLError)
    {
        std::cerr << "Error trying to changing inuse on ipsum, invalid XML in get entity" << std::endl;
        delete packet;
    }
}

void Ipsum::changeFrequencyHandler(IpsumChangeFreqPacket * packet)
{
    #ifdef IPSUM_DEBUG
    std::cout << "Ipsum::changeInUseHandler(IpsumChangeInUsePacket * packet)" << std::endl;
    #endif
    try
    {
        http->changeFreq(packet);
        localIpsumSendQueue->pop();
        delete packet;
    }
    catch(HttpError)
    {
        std::cerr << "Error trying to changing change frequency" << std::endl;
        try
        {
            http->ipsumInfo();
        }
        catch (HttpError)
        {
            IpsumUnreachable = true;
        }
    }
    catch(InvalidXMLError)
    {
        std::cerr << "Error trying to changing inuse on ipsum, invalid XML in get entity" << std::endl;
        delete packet;
    }

}
