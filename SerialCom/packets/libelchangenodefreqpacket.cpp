#include "libelchangenodefreqpacket.h"

LibelChangeNodeFreqPacket::LibelChangeNodeFreqPacket(std::vector<unsigned char> zigbeeAddress64bit, int newFrequency, unsigned char frameID) : TransmitRequestPacket()
{
    std::vector<unsigned char> data;

    data.push_back(newFrequency / 256);
    data.push_back(newFrequency % 256);

    setData(0x05, zigbeeAddress64bit, data, frameID);
}
