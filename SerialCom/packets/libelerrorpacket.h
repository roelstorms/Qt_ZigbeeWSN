#ifndef LIBELERRORPACKET_H
#define LIBELERRORPACKET_H
#include "receivepacket.h"
#include <map>
#include "errors.h"

class LibelErrorPacket : public ReceivePacket
{
private:
    std::map<int, std::string> errors;
public:
    LibelErrorPacket(std::vector<unsigned char> input) throw (ZbCorruptedPacket);
    std::string getErrorMsg() const;


    PacketType getPacketType(){ return ZB_LIBEL_ERROR; };

};


#endif // LIBELERRORPACKET_H
