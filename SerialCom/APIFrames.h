#ifndef APIFrames_H
#define APIFrames_H


/*
 * 	Written by Roel Storms
 * 	Depreciated, has been replaced by the different packet classes
 */

#include <string>
#include <boost/thread.hpp>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include <fstream>
#include "ATCommandResponsePacket.h"

class APIFrames
{

	private:
	APIFrames();
	~APIFrames();

	public:
	static std::vector<unsigned char> API_wrap(std::vector<unsigned char> input);
	static std::vector<unsigned char> ATCommand(std::vector<unsigned char> ATCommand, std::vector<unsigned char> parameter, bool applyChanges = true);
	static std::vector<unsigned char> remoteATCommand(std::vector<unsigned char> ATCommand, std::vector<unsigned char> parameter, std::vector<unsigned char> destinationAddress, std::vector<unsigned char> networkAddress, bool applyChanges = true);
	static std::vector<unsigned char> data(std::vector<unsigned char> data, std::vector<unsigned char> destinationAddress, std::vector<unsigned char> networkAddress, unsigned char broadcastRadius = 0);
	static Packet API_unwrap(std::vector<unsigned char> input);	
	static Packet ATCommandResponse(std::vector<unsigned char> input, Packet outputPacket);
	static Packet IODataSample(std::vector<unsigned char> input, Packet outputPacket);
};
#endif
