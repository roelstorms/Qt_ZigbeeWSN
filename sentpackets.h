/*
 *  Created by Roel Storms
 *
 *  This is a special templated queue that is used to keep pointers to outgoing zigbee packets which we expect a reply to.
 *  Packets in this queue can be resend if a reply doesn't come in before the expiration time. These packets should only be resent
 *  a couple of times as defined by numberOfRetries. Afterwards they are thrown away. Letting the client know about this failure
 *  should still be implemented. But for now our only communication with the client is Ipsum and there is no mechanism to report
 *  errors yet.
 *
 */

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
    std::vector<P> sentPackets;     // Packet + timestamp
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
    P findResendablePacket(unsigned char frameID);
};

template <class P, class R>
SentPackets<P,R>::SentPackets(unsigned char numberOfRetries, int expirationTime) : numberOfRetries(numberOfRetries), expirationTime(expirationTime)
{
    std::cout << "SentZBPackets<P,R>::SentZBPackets()" << std::endl;
}

template <class P, class R>
void SentPackets<P, R>::addPacket(P packet)
{
    packet->setTimeOfLastSending(0);
    sentPackets.push_back(packet);
}

template <class P, class R>
P SentPackets<P, R>::retrieveCorrespondingPacket(R packet)
{
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if(packet->correspondsTo((*it)))
        {
            return (*it);
        }
    }
    return nullptr;
}

template <class P, class R>
void SentPackets<P, R>::removePacket(P packet)
{
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if((*it) == packet)
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
    std::vector<P> resendablePackets;

    int currentTime = time(NULL);
    sentPackets.erase(std::remove_if (sentPackets.begin(), sentPackets.end(), [this, &currentTime, &resendablePackets, queue](P it){
        if(((currentTime-it->getTimeOfLastSending()) > expirationTime) && ( it->getTimeOfLastSending() != 0) && ((it->getNumberOfResends()) > numberOfRetries))       // True when packet expired and number of retries reached
        {
            delete it;
            return true;
        }
        else if (((currentTime - it->getTimeOfLastSending())  > expirationTime) && ( it->getTimeOfLastSending() != 0) && ((it->getNumberOfResends()) <= numberOfRetries))    // True when packet expired and number of retries not reached -> resend
        {
            std::cout << "expired packet found and resend done" << std::endl;
            resendablePackets.push_back(it);
            return false;
        }
        else
        {
            return false;
        }

    }), sentPackets.end());

    return resendablePackets;
}


template <class P, class R>
P SentPackets<P, R>::findResendablePacket(unsigned char frameID)
{
    P resendablePacket = nullptr;
    int lastTime = 0;
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if(((*it)->getFrameID() == frameID) && ((*it)->getNumberOfResends() <= numberOfRetries) && ((*it)->getTimeOfLastSending() > lastTime))
        {
            lastTime = (*it)->getTimeOfLastSending() ;
            resendablePacket = (*it);
            (*it)->incrementNumberOfResends();
        }
    }
    return resendablePacket;
}

#endif // SENTZBPACKETS_H
