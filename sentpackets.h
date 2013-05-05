#ifndef SENTZBPACKETS_H
#define SENTZBPACKETS_H
#include "packet.h"
#include <vector>
#include <time.h>
#include <iostream>
#include <stdlib.h>

template <class P, class R> // P for packet, R for response

class SentPackets
{
private:
    std::vector<std::pair<P, int> > sentPackets;     // Packet + timestamp
public:
    SentPackets();
    void addPacket(P packet);
    P  retrieveCorrespondingPacket(R packet);
    void removePacket(P packet);
    std::vector<P> findExpiredPacket(int expirationTime);
};

template <class P, class R>
SentPackets<P,R>::SentPackets()
{
    std::cout << "SentZBPackets<P,R>::SentZBPackets()" << std::endl;
}

template <class P, class R>
void SentPackets<P, R>::addPacket(P packet)
{
    int currentTime = time(NULL);
    sentPackets.push_back(std::pair<P, int> (packet, currentTime));

    std::cout << "Packet added to sentZBPacket with time: " << std::to_string(currentTime) << std::endl;
}

template <class P, class R>
P SentPackets<P, R>::retrieveCorrespondingPacket(R packet)
{
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if(packet->correspondsTo(it->first))
        {
            return it->first;
        }
    }
    return nullptr;
}

template <class P, class R>
void SentPackets<P, R>::removePacket(P packet)
{
    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if(it->first == packet)
        {
             sentPackets.erase(it);
             delete packet;
             return;
        }

    }
}

template <class P, class R>
std::vector<P> SentPackets<P, R>::findExpiredPacket(int expirationTime)
{
    std::vector<P> expiredPackets;

    for(auto it = sentPackets.begin(); it < sentPackets.end(); ++it)
    {
        if(it->second > expirationTime)     // Since adding packets happens by push_back() oldest packets will be returned first
                                            // If 2 simular packets have been sent we assume the oldest one gets a reply first.
        {
            expiredPackets.push_back(it->first);
            sentPackets.erase(it);
        }
    }
    return expiredPackets;
}
#endif // SENTZBPACKETS_H
