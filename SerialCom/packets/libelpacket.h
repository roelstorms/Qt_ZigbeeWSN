#ifndef LIBELPACKET_H
#define LIBELPACKET_H

class LibelPacket
{
	private:

	public:
	virtual std::vector<unsigned char> getMask() const = 0;
	virtual std::vector<unsigned char> getZigbee16BitAddress() const = 0;
	virtual std::vector<unsigned char> getZigbee64BitAddress() const = 0;
	virtual std::vector<unsigned char> getRFData() const throw (ZbCorruptedFrameData) = 0;
	virtual std::vector<unsigned char> getData() const throw (ZbCorruptedFrameData) = 0;

};

#endif
