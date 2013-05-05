#ifndef IPSUMCHANGEFREQPACKET_H
#define IPSUMCHANGEFREQPACKET_H

#include <vector>
#include "ipsumpacket.h"
#include "../enums.h"

class IpsumChangeFreqPacket : public IpsumPacket
{
	private:
	int installationID, sensorGroupID;
    std::vector<std::pair<int, int>> frequencies;   //SensorID and new frequency (frequency is actually the interval in 10s between sampling

	public:

	// Parameters: data is a tuple of sensor type, sensorID as used in ipsum and a float with the data
    IpsumChangeFreqPacket(int installationID, int sensorGroupID, std::vector<std::pair<int, int>> frequencies);

	virtual PacketType getPacketType(){ return IPSUM_CHANGE_FREQ;};
	
	int getInstallationID() const;
	int getSensorGroupID() const;

    const std::vector<std::pair<int, int>>& getFrequencies() const;
};

#endif
