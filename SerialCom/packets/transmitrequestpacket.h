#ifndef TRANSMITREQUESTPACKET_H
#define TRANSMITREQUESTPACKET_H
#include "outgoingpacket.h"
#include "libelpacket.h"
#include "../../enums.h"
#include "../../errors.h"
#include <sstream>

class TransmitRequestPacket : public  OutgoingPacket, public LibelPacket 
{

	private:

	public:
        TransmitRequestPacket();

        void setData(unsigned char applicationID, std::vector<unsigned char> zigbeeAddress64Bit, std::vector<unsigned char> data);
        void setRFData(std::vector<unsigned char> zigbeeAddress64Bit, std::vector<unsigned char> applicationData);

		virtual std::vector<unsigned char> getMask() const;
		virtual std::vector<unsigned char> getZigbee16BitAddress() const;
		virtual std::vector<unsigned char> getZigbee64BitAddress() const ;
		virtual std::vector<unsigned char> getRFData() const throw (ZbCorruptedFrameData);
        virtual std::vector<unsigned char> getData() const throw(ZbCorruptedFrameData);

};

#endif

