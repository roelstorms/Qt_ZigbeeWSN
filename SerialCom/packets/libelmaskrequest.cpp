#include "libelmaskrequest.h"

LibelMaskRequest::LibelMaskRequest(std::vector<unsigned char> zigbeeAddress64bit, unsigned char frameID) : TransmitRequestPacket()
{
    std::vector<unsigned char> data;

		
    setData(0x03, zigbeeAddress64bit, data, frameID);
}

