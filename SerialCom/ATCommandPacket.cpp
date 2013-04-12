#include "ATCommandPacket.h"


ATCommandPacket::ATCommandPacket(std::vector<unsigned char> aATCommand, std::vector<unsigned char> aParameter, int aFrameID, bool aApplyChangesEnabled) : Packet(), frameID(aFrameID), parameter(aParameter), applyChangesEnabled(aApplyChangesEnabled), ATCommand(aATCommand)
{
	std::vector<unsigned char>unwrappedPacket;
	if(aATCommand.size() == 2)
	{
		if(aApplyChangesEnabled)
		{
			unwrappedPacket.push_back(0x08);
		}
		else
		{
			unwrappedPacket.push_back(0x09);
		}

		unwrappedPacket.push_back(aFrameID);
		if(aATCommand.size() != 2)
		{
			std::cout << "ATCommand length is invalid" << std::endl;
			return;
		}
		unwrappedPacket.push_back(aATCommand.at(0));
		unwrappedPacket.push_back(aATCommand.at(1));
		for(int i = 0; i < aParameter.size(); ++i)
		{
			unwrappedPacket.push_back(aParameter.at(i));
		}
		wrap(unwrappedPacket);
	}
	else
	{
		std::cout << "Length of AT command was incorrect" << std::endl;	// Length of AT command is not correct
	}

}



const std::vector<unsigned char>& ATCommandPacket::getATCommand() const
{
	return ATCommand; 
}

const std::vector<unsigned char>& ATCommandPacket::getParamater() const
{
	return parameter;
}

const bool& ATCommandPacket::getApplyChangedEnabled() const
{
	return applyChangesEnabled;
}

const int& ATCommandPacket::getFrameID() const
{
	return frameID;
}
