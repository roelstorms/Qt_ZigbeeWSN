#include "ipsumuploadpacket.h"


IpsumUploadPacket::IpsumUploadPacket(int installationID, int sensorGroupID, std::vector<std::tuple<SensorType, int, float>> data) : IpsumPacket(), installationID(installationID), sensorGroupID(sensorGroupID), data(data)
{


}

int IpsumUploadPacket::getInstallationID() const
{
	return installationID;
}

int IpsumUploadPacket::getSensorGroupID() const
{
	return sensorGroupID;
}

const std::vector<std::tuple<SensorType, int, float>>& IpsumUploadPacket::getData() const
{
	return data;
}
