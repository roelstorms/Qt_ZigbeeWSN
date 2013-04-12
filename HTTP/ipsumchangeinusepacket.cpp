#include "ipsumchangeinusepacket.h"


IpsumChangeInUsePacket::IpsumChangeInUsePacket(int installationID, int sensorGroupID, bool inUse) : IpsumPacket(), installationID(installationID), sensorGroupID(sensorGroupID), inUse(inUse)
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
