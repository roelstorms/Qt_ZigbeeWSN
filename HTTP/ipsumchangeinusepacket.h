#ifndef IPSUMCHANGEINUSEPACKET_H
#define IPSUMCHANGEINUSEPACKET_H

#include <vector>
#include "ipsumpacket.h"
#include "../enums.h"

class IpsumChangeInUsePacket : public IpsumPacket
{
	private:
	int installationID, sensorGroupID;
	bool inUse;
	public:

	// Parameters: data is a tuple of sensor type, sensorID as used in ipsum and a float with the data
	IpsumChangeInUsePacket(int installationID, int sensorGroupID, bool inUse);

	virtual PacketType getPacketType(){ return IPSUM_CHANGE_IN_USE; };
	
	int getInstallationID() const;
	int getSensorGroupID() const;
	bool getInUse() const;
};

#endif
