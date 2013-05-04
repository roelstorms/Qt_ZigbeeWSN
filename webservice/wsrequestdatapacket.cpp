#include "wsrequestdatapacket.h"

WSRequestDataPacket::WSRequestDataPacket(std::string data)
{
    XML XMLParser;
    xercesc::DOMDocument * doc = XMLParser.parseToDom( data );
    char * temp;

    xercesc::DOMElement * docElement = doc->getDocumentElement();
    xercesc::DOMElement * nextElement;
    nextElement = docElement->getFirstElementChild();

    int sensorGroupID = -1;

    XMLCh * sensorString = xercesc::XMLString::transcode("sensorID");
    XMLCh * sensorGroupIDString = xercesc::XMLString::transcode("sensorGroupID");

    std::vector<int> sensors;      // IpsumID + frequency (interval) in seconds

    while(nextElement != NULL)
    {
        if(xercesc::XMLString::compareIString(nextElement->getTagName(), sensorGroupIDString) == 0)
        {
            temp = xercesc::XMLString::transcode(nextElement->getTextContent());
            sensorGroupID = boost::lexical_cast<int>(std::string(temp));
            xercesc::XMLString::release(&temp);
        }
        else if(xercesc::XMLString::compareIString(nextElement->getTagName(), sensorString) == 0)
        {

                temp = xercesc::XMLString::transcode(nextElement->getTextContent());
                sensors.push_back(boost::lexical_cast<int>(temp));
                xercesc::XMLString::release(&temp);

        }
        else
        {
            throw InvalidWSXML();
        }
        nextElement = nextElement->getNextElementSibling();
    }
}

const int &WSRequestDataPacket::getSensorGroupID() const
{
    return sensorGroupID;
}

const std::vector<int> & WSRequestDataPacket::getSensors() const
{
    return sensors;
}
