#ifndef ATCOMMANDPACKET_H 
#define ATCOMMANDPACKET_H 
 
#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include "packet.h"
#include <iomanip>

class ATCommandPacket : public Packet
{
	private:
	std::vector<unsigned char> ATCommand;
	bool applyChangesEnabled;	// This decides if the packet is of type 08 of 09. The 09 packet will not apply changes autmaticall. See zigbee API documantation for details.
	std::vector<unsigned char> parameter;
	int frameID;
	public:
	ATCommandPacket(std::vector<unsigned char> aATCommand, std::vector<unsigned char> aParameter, int aFrameID, bool aApplyChangesEnabled);

	const std::vector<unsigned char>& getATCommand() const;
	const std::vector<unsigned char>& getParamater() const;
	const bool& getApplyChangedEnabled() const;
	const int& getFrameID() const;
};

#endif
