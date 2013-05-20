#ifndef SENTZBPACKETS_H
#define SENTZBPACKETS_H
#include "packet.h"
#include <vector>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <tuple>
#include "queue"

template <class P, class R> // P for packet, R for response

class SentPackets
{
private:
    std::vector<std::tuple<P, int, int> > sentPackets;     // Packet + timestamp + number of retries
    unsigned char numberOfRetries;
    int expirationTime;
public:
    SentPackets(unsigned char numberOfRetries, int expirationTime);
    void addPacket(P packet);


    P retrieveCorrespondingPacket(R packet);

    /*
     *  When a packet which is sent receives a reply it can be removed from the sent packets list.
     */
    void removePacket(P packet);

    /*
     *  Returns a vector of packets that have been sent and have not received a reply.
     *  These packets could also be resend.
     */
    std::vector<P> findExpiredPacket(std::vector<Packet *> *queue);

    /*
     *  Finds packets with matching frameID. If more packets have the same frameID this means the frameID has overflowed and the newest sent packet is returned.
     *  Since it is probably the last sent packet that received an acknoledgement. Also the time of sending this packet is returned to compare it with other packets from other sent queues.
     */
    std::pair<P, int> findResendablePacket(unsigned char frameID);

    //std::pair<P, int> findResendablePacket(P packet);
};

template <class P, class R>
SentPackets<P,R>::SentPackets(unsigned char numberOfRetries, int expirationTime) : numberOfRetries(numberOfRetries), expirationTime(expirationTime)
{
    std::cout << "SentZBPackets<P,R>::SentZBPackets()" << std::endl;
}

template <class P, class R>
void SentPackets<P, R>::addPacket(P packet)
{
    int currentTime = time(NULL);
    sentPackets.push_back(std::tuple<P, int, int> (packet, currentTime, 0));

    std::cout << "Packet added to sentZBPacket with time: " << std::to_string(currentTime) << std::endl;
}

template <class P, class R>
P SentPackets<P, R>::retrieveCorrespondingPacket(R packet)
{
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if(packet->correspondsTo(std::get<0>(*it)))
        {
            return (std::get<0>(*it));
        }
    }
    return nullptr;
}

template <class P, class R>
void SentPackets<P, R>::removePacket(P packet)
{
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if(std::get<0>(*it) == packet)
        {
             sentPackets.erase(it);     // This is safe since we stop iterating after the erase. Else you can't erase it because then ++it becomes invalid.
             delete packet;
             return;
        }

    }
}

template <class P, class R>
std::vector<P> SentPackets<P, R>::findExpiredPacket(std::vector<Packet *> * queue)
{
    std::vector<P> expiredPackets;
    sentPackets.erase(std::remove_if (sentPackets.begin(), sentPackets.end(), [this, &expiredPackets, queue](std::tuple<P, int,int> it){
        if((std::get<1>(it) > expirationTime) && (std::get<2>(it) > numberOfRetries))       // True when packet expired and number of retries reached
        {
            expiredPackets.push_back(std::get<0>(it));
            return true;
        }
        else if ((std::get<1>(it) > expirationTime) && (std::get<2>(it) <= numberOfRetries))    // True when packet expired and number of retries not reached -> resend
        {
            std::cout << "expired packet found and resend done" << std::endl;
            //queue->push_back(std::get<0>(it));
            return false;
        }
        else
        {
            return false;
        }

    }), sentPackets.end());

    return expiredPackets;
}


template <class P, class R>
std::pair<P, int> SentPackets<P, R>::findResendablePacket(unsigned char frameID)
{
    std::pair <P, int> resendablePacket(nullptr, 0);
    int lastTime = 0;
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if((std::get<0>(*it)->getFrameID() == frameID) && (std::get<2>(*it) <= numberOfRetries) && (std::get<1>(*it) > lastTime))
        {
            lastTime = std::get<1>(*it);
            resendablePacket = std::pair<P, int> (std::get<0>(*it), std::get<1>(*it));
            ++std::get<2>(*it);
        }
    }
    return resendablePacket;
}

/*
template <class P, class R>
std::pair<P, int> SentPackets<P, R>::findResendablePacket(P packet)
{
    std::pair <P, int> resendablePacket(nullptr, 0);
    int lastTime = 0;
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if((std::get<0>(*it) == packet) && (std::get<2>(*it) <= numberOfRetries) && (std::get<1>(*it) > lastTime))
        {
            lastTime = std::get<1>(*it);
            resendablePacket = std::pair<P, int> (std::get<0>(*it), std::get<1>(*it));
            ++std::get<2>(*it);
        }
    }
    return resendablePacket;
}
*/
#endif // SENTZBPACKETS_H
