#include "ipsumchangeinusepacket.h"


IpsumChangeInUsePacket::IpsumChangeInUsePacket(int installationID, int sensorGroupID, std::map<int, bool> sensors, bool inUse) : IpsumPacket(), installationID(installationID), sensorGroupID(sensorGroupID), sensors(sensors), inUse(inUse)
{


}

int IpsumChangeInUsePacket::getInstallationID() const
{
	return installationID;
}

int IpsumChangeInUsePacket::getSensorGroupID() const
{
	return sensorGroupID;
}

bool IpsumChangeInUsePacket::getInUse() const
{

    return inUse;
}

const std::map<int, bool> &IpsumChangeInUsePacket::getSensors() const
{
    return sensors;
}
