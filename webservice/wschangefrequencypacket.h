#ifndef WSCHANGEFREQUENCYPACKET_H
#define WSCHANGEFREQUENCYPACKET_H

#include "wspacket.h"

class WSChangeFrequencyPacket : public WSPacket
{
private:
    int sensorGroupID;
    std::vector<std::pair<int, int> > frequencies;      // IpsumID + frequency (interval) in seconds
public:
    WSChangeFrequencyPacket(std::string data);


    PacketType getPacketType(){ return WS_CHANGE_FREQUENCY_COMMAND; };

    const int& getSensorGroupID() const;
    const std::vector<std::pair<int, int> >& getFrequencies() const;
};

#endif // WSCHANGEFREQUENCYPACKET_H
