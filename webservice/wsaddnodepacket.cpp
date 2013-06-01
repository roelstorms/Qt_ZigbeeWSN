#include "wsaddnodepacket.h"

WSAddNodePacket::WSAddNodePacket(std::string data) throw(InvalidXMLError) : WSPacket()
{
    XML XMLParser;

    xercesc::DOMDocument * doc = XMLParser.parseToDom(data);

    char * temp;

    xercesc::DOMElement * docElement = doc->getDocumentElement();
    if(docElement == NULL)
        throw InvalidXMLError();

    xercesc::DOMElement * nextElement;
    nextElement = docElement->getFirstElementChild();

    XMLCh * installationIDString = xercesc::XMLString::transcode("installationID");
    XMLCh * sensorGroupIDString = xercesc::XMLString::transcode("sensorGroupID");
    XMLCh * zigbeeAddressString = xercesc::XMLString::transcode("zigbeeAddress");
    while(nextElement != NULL)
    {
        if(xercesc::XMLString::compareIString(nextElement->getTagName(), installationIDString) == 0)
        {
            temp = xercesc::XMLString::transcode(nextElement->getTextContent());
            installationID = boost::lexical_cast<int> (std::string(temp));
            xercesc::XMLString::release(&temp);

        }
        else if(xercesc::XMLString::compareIString(nextElement->getTagName(), sensorGroupIDString) == 0)
        {
            temp = xercesc::XMLString::transcode(nextElement->getTextContent());
            sensorGroupID = boost::lexical_cast<int> (std::string(temp));
            xercesc::XMLString::release(&temp);

        }
        else if(xercesc::XMLString::compareIString(nextElement->getTagName(), zigbeeAddressString) == 0)
        {
            temp = xercesc::XMLString::transcode(nextElement->getTextContent());
            zigbeeAddress64Bit = std::string(temp);
            xercesc::XMLString::release(&temp);
        }
        else
        {
            std::cerr << "invalid XML: " << std::endl;
            std::cerr << "textContent of invalid xml: " << std::string(xercesc::XMLString::transcode(nextElement->getTextContent())) << std::endl;
            std::cerr << "tagname of invalid XML: " << std::string(xercesc::XMLString::transcode(nextElement->getTagName())) << std::endl;
            throw InvalidWSXML();
        }

        nextElement = nextElement->getNextElementSibling();
    }

    xercesc::XMLString::release( &installationIDString );
    xercesc::XMLString::release( &sensorGroupIDString );
    xercesc::XMLString::release( &zigbeeAddressString );
}

const int &WSAddNodePacket::getInstallationID() const
{
    return installationID;
}

const int &WSAddNodePacket::getSensorGroupID() const
{
    return sensorGroupID;
}

const std::string &WSAddNodePacket::getZigbeeAddress64Bit() const
{
    return zigbeeAddress64Bit;
}

