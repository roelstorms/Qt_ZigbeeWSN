#ifndef IPSUMUPLOADPACKET_H
#define IPSUMUPLOADPACKET_H

#include <vector>
#include "ipsumpacket.h"
#include "../enums.h"
#include <tuple>

class IpsumUploadPacket : public IpsumPacket
{
	private:
	int installationID, sensorGroupID;
	std::vector<std::tuple<SensorType, int, float>> data;
	public:

	// Parameters: data is a tuple of sensor type, sensorID as used in ipsum and a float with the data
	IpsumUploadPacket(int installationID, int sensorGroupID, std::vector<std::tuple<SensorType, int, float>> data);

	virtual PacketType getPacketType(){ return IPSUM_UPLOAD;};
	
	int getInstallationID() const;
	int getSensorGroupID() const;

	const std::vector<std::tuple<SensorType, int, float>>& getData() const;
};

#endif
