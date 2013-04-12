#include "ipsumchangefreqpacket.h"


IpsumChangeFreqPacket::IpsumChangeFreqPacket(int installationID, int sensorGroupID, std::vector<std::tuple<SensorType, int, int>> frequencies) : IpsumPacket(), installationID(installationID), sensorGroupID(sensorGroupID), frequencies(frequencies)
{


}

int IpsumChangeFreqPacket::getInstallationID() const
{
	return installationID;
}

int IpsumChangeFreqPacket::getSensorGroupID() const
{
	return sensorGroupID;
}

const std::vector<std::tuple<SensorType, int, int>>& IpsumChangeFreqPacket::getFrequencies() const
{
	return frequencies;
}
