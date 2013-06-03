#include "ipsum.h"

Ipsum::Ipsum(bool * stop, std::string ipsumURL, std::string ipsumPersonalKey, PacketQueue * ipsumSendQueue, PacketQueue * ipsumReceiveQueue, std::mutex * mainConditionVariableMutex, std::condition_variable * mainConditionVariable, std::mutex * ipsumConditionVariableMutex, std::condition_variable * ipsumConditionVariable) : stop(stop), ipsumSendQueue(ipsumSendQueue), ipsumReceiveQueue(ipsumReceiveQueue), mainConditionVariableMutex(mainConditionVariableMutex), ipsumConditionVariableMutex(ipsumConditionVariableMutex), mainConditionVariable(mainConditionVariable), ipsumConditionVariable(ipsumConditionVariable), IpsumUnreachable(false)
{
	std::cout << "ipsum constructor" << std::endl;
	localIpsumSendQueue = new std::queue<Packet*>;
    http = new Http();
}

Ipsum::~Ipsum()
{
	delete http;
	delete localIpsumSendQueue;
}

void Ipsum::operator()()
{
	std::cout << "Ipsum::operator()()" << std::endl;
    while(!(*stop))
    {
        std::unique_lock<std::mutex> uniqueLock(*ipsumConditionVariableMutex);
        ipsumConditionVariable->wait(uniqueLock, [this] {return (!ipsumSendQueue->empty() || (*stop));});

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
                    std::cerr << "packet type not recognized in ipsum thread" << std::endl;
                    // Packet not recognized

                }
            }
        }
        else if(*stop)
        {
            /*  Ipsum thread is going to exit but Ipsum has been unreachable. Data will be lost here
             *  Solution is to send the packets in the local queue back to main and main should store them in sql db somehow.
             *  Storing them in main could be done by using the boost serialization library http://www.boost.org/doc/libs/1_37_0/libs/serialization/doc/index.html.
             *  This might be too heavyweight so a system to store the relevant info in the sql db in main is a better solution
             */
            std::cerr << "Ipsum exited with cached packets in the local queue, these packets are now lost" << std::endl;
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
    std::cout << "Ipsum::changeFrequencyHandler(IpsumChangeFreqPacket * packet)" << std::endl;
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
