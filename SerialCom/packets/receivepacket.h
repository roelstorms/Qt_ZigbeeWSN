#ifndef RECEIVEPACKET_H 
#define RECEIVEPACKET_H 
 
#include <string>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include "incomingpacket.h"
#include "libelpacket.h"
#include <iomanip>

class ReceivePacket : public IncomingPacket, public LibelPacket
{
	private:

	public:
	ReceivePacket(std::vector<unsigned char> input);

	void unescapeData();


	void setRFData(std::vector<unsigned char> rfData);
	
	virtual std::vector<unsigned char> getMask() const;
	virtual std::vector<unsigned char> getZigbee16BitAddress() const;
	virtual std::vector<unsigned char> getZigbee64BitAddress() const ;
	virtual std::vector<unsigned char> getRFData() const throw (ZbCorruptedFrameData);
    virtual std::vector<unsigned char> getData() const throw (ZbCorruptedFrameData);
    unsigned char getFrameID() const;
};

#endif
