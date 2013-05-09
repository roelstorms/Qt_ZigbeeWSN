#include "wsaddsensorspacket.h"

WSAddSensorsPacket::WSAddSensorsPacket(std::string data)
{
    char * temp;

    XML XMLParser;
    xercesc::DOMDocument * doc = XMLParser.parseToDom(data);

    sensorGroupID = -1;

    xercesc::DOMElement * docElement = doc->getDocumentElement();
    xercesc::DOMElement * nextElement;
    nextElement = docElement->getFirstElementChild();

    XMLCh * sensorGroupIDString = xercesc::XMLString::transcode("sensorGroupID");
    XMLCh * sensorString = xercesc::XMLString::transcode("sensor");
    XMLCh * sensorIDString = xercesc::XMLString::transcode("sensorID");
    XMLCh * sensorTypeString = xercesc::XMLString::transcode("sensorType");

    while(nextElement != NULL)
    {
        if(xercesc::XMLString::compareIString(nextElement->getTagName(), sensorGroupIDString) == 0)
        {
            temp = xercesc::XMLString::transcode(nextElement->getTextContent());
            sensorGroupID = boost::lexical_cast<int>(temp);
            xercesc::XMLString::release(&temp);

        }
        if(xercesc::XMLString::compareIString(nextElement->getTagName(), sensorString) == 0)
        {
            std::pair<SensorType, int> sensor;
            xercesc::DOMElement * child;
            child = nextElement->getFirstElementChild();

            if(xercesc::XMLString::compareIString(child->getTagName(), sensorIDString) == 0)
            {
                temp = xercesc::XMLString::transcode(child->getTextContent());
                sensor.second = boost::lexical_cast<int>(temp);
                xercesc::XMLString::release(&temp);

            }
            else
            {
                throw InvalidWSXML();
            }

            child = child->getNextElementSibling();

            if(xercesc::XMLString::compareIString(child->getTagName(), sensorTypeString) == 0)
            {
                temp = xercesc::XMLString::transcode(child->getTextContent());
                std::cout << "sensorType in XML from WS: " << temp << std::endl;
                sensor.first = stringToSensorType(temp);

                xercesc::XMLString::release(&temp);
            }
            else
            {
                throw InvalidWSXML();
            }
            std::cout << "adding sensor to vector" << std::endl;
            sensors.insert(sensor);

        }

        nextElement = nextElement->getNextElementSibling();
    }
}

const int & WSAddSensorsPacket::getSensorGroupID() const
{
    return sensorGroupID;
}

const std::map<SensorType, int>& WSAddSensorsPacket::getSensors() const
{
    return sensors;
}


SensorType WSAddSensorsPacket::stringToSensorType(std::string sensorType) throw (InvalidWSXML)
{
    std::cout << "stringToSensorType" << std::endl;
    if(sensorType == "zigbeeTemp")
    {
        return TEMP;
    }
    else if (sensorType == "zigbeeHum")
    {
        return HUM;
    }
    else if (sensorType == "zigbeePres")
    {
        return PRES;
    }
    else if (sensorType == "zigbeeBat")
    {
        return BAT;
    }
    else if (sensorType == "zigbeeCO2")
    {
        return CO2;
    }
    else if (sensorType == "zigbeeAnemo")
    {
        return ANEMO;
    }
    else if (sensorType == "zigbeeVane")
    {
        return VANE;
    }
    else if (sensorType == "zigbeePluvio")
    {
        return PLUVIO;
    }
    else
    {
        throw InvalidWSXML();
    }
}
