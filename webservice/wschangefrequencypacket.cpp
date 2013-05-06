#include "wschangefrequencypacket.h"

WSChangeFrequencyPacket::WSChangeFrequencyPacket(std::string data)
{
    char * temp;
    XML XMLParser;
    xercesc::DOMDocument * doc = XMLParser.parseToDom(data);

    xercesc::DOMElement * docElement = doc->getDocumentElement();
    xercesc::DOMElement * nextElement;
    nextElement = docElement->getFirstElementChild();

    sensorGroupID = -1;

    XMLCh * sensorString = xercesc::XMLString::transcode("sensor");
    XMLCh * sensorIDString = xercesc::XMLString::transcode("sensorID");
    XMLCh * sensorGroupIDString = xercesc::XMLString::transcode("sensorGroupID");
    XMLCh * frequencyString = xercesc::XMLString::transcode("frequency");

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
            std::pair<int, int> freq;
            xercesc::DOMElement * child;
            child = nextElement->getFirstElementChild();

            if(xercesc::XMLString::compareIString(child->getTagName(), sensorIDString) == 0)
            {
                temp = xercesc::XMLString::transcode(child->getTextContent());
                freq.first = boost::lexical_cast<int>(temp);
                xercesc::XMLString::release(&temp);

            }
            else
            {
                throw InvalidWSXML();
            }

            child = child->getNextElementSibling();

            if(xercesc::XMLString::compareIString(child->getTagName(), frequencyString) == 0)
            {
                temp = xercesc::XMLString::transcode(child->getTextContent());
                freq.second = boost::lexical_cast<int> (temp);

                xercesc::XMLString::release(&temp);
            }
            else
            {
                throw InvalidWSXML();
            }
            std::cout << "adding sensor to vector" << std::endl;
            frequencies.push_back(freq);
        }
        else
        {
            std::cerr << "invalid XML" << std::endl;
            throw InvalidWSXML();
        }

        nextElement = nextElement->getNextElementSibling();
    }
}

const int &WSChangeFrequencyPacket::getSensorGroupID() const
{
    return sensorGroupID;
}

const std::vector<std::pair<int, int> > &WSChangeFrequencyPacket::getFrequencies() const
{
    return frequencies;
}
