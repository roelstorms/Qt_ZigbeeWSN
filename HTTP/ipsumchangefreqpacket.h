#ifndef IPSUMCHANGEFREQPACKET_H
#define IPSUMCHANGEFREQPACKET_H

#include <vector>
#include "ipsumpacket.h"
#include "../enums.h"
#include <tuple>

class IpsumChangeFreqPacket : public IpsumPacket
{
	private:
	int installationID, sensorGroupID;
	std::vector<std::tuple<SensorType, int, int>> frequencies;
	public:

	// Parameters: data is a tuple of sensor type, sensorID as used in ipsum and a float with the data
	IpsumChangeFreqPacket(int installationID, int sensorGroupID, std::vector<std::tuple<SensorType, int, int>> frequencies);

	virtual PacketType getPacketType(){ return IPSUM_CHANGE_FREQ;};
	
	int getInstallationID() const;
	int getSensorGroupID() const;

	const std::vector<std::tuple<SensorType, int, int>>& getFrequencies() const;
};

#endif
