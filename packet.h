#ifndef PACKET_H
#define PACKET_H
#include "errors.h"
#include <cstddef>

enum PacketType		
{
	ZB_IO, ZB_TRANSMIT_REQUEST, ZB_LIBEL_MASK_REQUEST, ZB_LIBEL_MASK_RESPONSE, ZB_LIBEL_IO, ZB_LIBEL_REQUEST_IO, ZB_LIBEL_CHANGE_FREQ,
       	ZB_LIBEL_CHANGE_FREQ_RESPONSE,ZB_LIBEL_CHANGE_NODE_FREQ, ZB_LIBEL_CHANGE_NODE_FREQ_RESPONSE, ZB_LIBEL_ADD_NODE, 
	ZB_LIBEL_ADD_NODE_RESPONSE, 

    WS_COMMAND, WS_ADD_NODE_COMMAND, WS_CHANGE_FREQUENCY_COMMAND, WS_ADD_SENSORS_COMMAND, WS_REQUEST_DATA_COMMAND,

	IPSUM_UPLOAD, IPSUM_CHANGE_IN_USE, IPSUM_CHANGE_FREQ,
	first = ZB_LIBEL_IO, last = WS_COMMAND
};


class Packet
{
	private:
		Packet(Packet&);
		Packet(const Packet&);
		Packet& operator=(const Packet&);
	public:
		Packet() throw (InvalidPacketType);
        virtual ~Packet(){};
		// Purpose of this function is to make packets bind virtually as needed by the dynamic cast to put packets in a queueu.
		virtual PacketType getPacketType() = 0;
};

#endif
