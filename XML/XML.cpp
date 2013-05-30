#include "XML.h"
XML::XML() throw (XercesError)
{
	std::cout << "XML constructor" << std::endl << std::endl;
	try {
		xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) {
        std::cerr << "xercesc::XMLPlatformUtils::Initialize() failed" << std::endl << std::endl;// Do your failure processing here
	}

	XMLCh tempStr[100];
	xercesc::XMLString::transcode("Core", tempStr, 99);
	impl = xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
	if(impl == NULL)
	{
        std::cerr << "no implementation found" << std::endl;
        throw XercesError();
	}


    parser = ((xercesc::DOMImplementationLS*)impl)->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);

}

XML::~XML()
{
    parser->release();

	xercesc::XMLPlatformUtils::Terminate();
}


std::string XML::serializeDOM(xercesc::DOMNode* node) {

	XMLCh tempStr[100];// = (XMLCh *) malloc(sizeof(XMLCh) * 100);
	std::string XMLOutput;

	xercesc::XMLString::transcode("LS", tempStr, 99);
	xercesc::DOMLSSerializer* theSerializer = ((xercesc::DOMImplementationLS*)impl)->createLSSerializer();

	// optionally you can set some features on this serializer
	if (theSerializer->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMWRTDiscardDefaultContent, true))
		theSerializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTDiscardDefaultContent, true);

	if (theSerializer->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true))
		theSerializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);

	// optionally you can implement your DOMLSSerializerFilter (e.g. MyDOMLSSerializerFilter)
	// and set it to the serializer
	//DOMLSSerializerFilter* myFilter = new myDOMLSSerializerFilter();
	//theSerializer->setFilter(myFilter);

	// optionally you can implement your DOMErrorHandler (e.g. MyDOMErrorHandler)
	// and set it to the serializer
	//DOMErrorHandler* myErrorHandler() = new myDOMErrorHandler();
	//theSerializer->getDomConfig()->setParameter(XMLUni::fgDOMErrorHandler, myErrorHandler);

	// StdOutFormatTarget prints the resultant XML stream
	// to stdout once it receives any thing from the serializer.

	xercesc::XMLFormatTarget *myFormTarget = new xercesc::StdOutFormatTarget();
	xercesc::DOMLSOutput* theOutput = ((xercesc::DOMImplementationLS*)impl)->createLSOutput();
	theOutput->setByteStream(myFormTarget);
	XMLCh * XMLTemp = theSerializer->writeToString(node);
	char * temp = xercesc::XMLString::transcode(XMLTemp);
	//xercesc::XMLString::release(&tempStr);
	XMLOutput.append(temp);
	xercesc::XMLString::release(&XMLTemp);
	xercesc::XMLString::release(&temp);

	try {
		// do the serialization through DOMLSSerializer::write();
		theSerializer->write(node, theOutput);
	}
	catch (const xercesc::XMLException& toCatch) {
		char* message = xercesc::XMLString::transcode(toCatch.getMessage());
        std::cerr << "Exception message is: \n"
			<< message << "\n";
		xercesc::XMLString::release(&message);
	}
	catch (const xercesc::DOMException& toCatch) {
		char* message = xercesc::XMLString::transcode(toCatch.msg);
        std::cerr << "Exception message is: \n" << message << "\n";
		xercesc::XMLString::release(&message);
	}
	catch (...) {
        std::cerr << "Unexpected Exception \n" ;
	}
	delete myFormTarget;
	theOutput->release();
	theSerializer->release();
	return XMLOutput;
}

std::string XML::uploadData(const std::string& type, const std::vector<std::pair<std::string, double>>& input)
{
	XMLCh tempStr[100];

	std::string XMLOutput;

    #ifdef XML_DEBUG
        std::cout << "begin of upload" << std::endl;
    #endif

	xercesc::XMLString::transcode("upload", tempStr, 99);
	xercesc::DOMDocument* doc = impl->createDocument(0, tempStr, 0);
	xercesc::DOMElement* upload = doc->getDocumentElement();


	xercesc::XMLString::transcode("utimestamp", tempStr, 99);
	xercesc::DOMElement* utimestamp = doc->createElement(tempStr);
	upload->appendChild(utimestamp);  

	
	
	xercesc::XMLString::transcode(getTimeInSeconds().c_str(), tempStr, 99);
	xercesc::DOMText* timestampValue = doc->createTextNode(tempStr);
	utimestamp->appendChild(timestampValue);

	xercesc::XMLString::transcode("items", tempStr, 99);
	xercesc::DOMElement* items = doc->createElement(tempStr);
	upload->appendChild(items);

	xercesc::XMLString::transcode(type.c_str(), tempStr, 99);
	xercesc::DOMElement* myType = doc->createElement(tempStr);
	items->appendChild(myType);

	for(auto it = input.begin(); it < input.end(); ++it)
	{		

		xercesc::XMLString::transcode((*it).first.c_str(), tempStr, 99);
		xercesc::DOMNode* field = doc->createElement(tempStr);
		myType->appendChild(field);

		std::ostringstream stream;		// Can use boost to convert double to string more elegantly
		stream << (*it).second;
		std::string fieldValue = stream.str();

		xercesc::XMLString::transcode(fieldValue.c_str(), tempStr, 99);
		xercesc::DOMText* fieldvalue = doc->createTextNode(tempStr);
		field->appendChild(fieldvalue);

		
	}
	xercesc::XMLString::transcode("utimestamp", tempStr, 99);
	xercesc::DOMNode* fieldTimestamp = doc->createElement(tempStr);
	myType->appendChild(fieldTimestamp);

	xercesc::XMLString::transcode("0", tempStr, 99);
	xercesc::DOMText* fieldTimestampValue = doc->createTextNode(tempStr);
	fieldTimestamp->appendChild(fieldTimestampValue);

	XMLOutput = serializeDOM(upload);

	doc->release();

    #ifdef XML_DEBUG
        std::cout << "XMLouput:" << std::endl << XMLOutput << std::endl;
    #endif

	// Other terminations and cleanup.
	return XMLOutput;
}

std::string XML::uploadData(const std::string& type, const std::string& fieldName, float data, std::string timeStamp)
{
	XMLCh tempStr[100];

	std::string XMLOutput;

    #ifdef XML_DEBUG
        std::cout << "begin of upload" << std::endl;
    #endif

	xercesc::XMLString::transcode("upload", tempStr, 99);
	xercesc::DOMDocument* doc = impl->createDocument(0, tempStr, 0);
	xercesc::DOMElement* upload = doc->getDocumentElement();


	xercesc::XMLString::transcode("utimestamp", tempStr, 99);
	xercesc::DOMElement* utimestamp = doc->createElement(tempStr);
	upload->appendChild(utimestamp);  


	unsigned long int sec = time(NULL);

    #ifdef XML_DEBUG
        std::cout << "time: " << std::endl << std::string(std::to_string(sec)) << std::endl;
    #endif
	xercesc::XMLString::transcode(timeStamp.c_str(), tempStr, 99);
	xercesc::DOMText* timestampValue = doc->createTextNode(tempStr);
	utimestamp->appendChild(timestampValue);



	xercesc::XMLString::transcode("items", tempStr, 99);
	xercesc::DOMElement* items = doc->createElement(tempStr);
	upload->appendChild(items);

	xercesc::XMLString::transcode(type.c_str(), tempStr, 99);
	xercesc::DOMElement* myType = doc->createElement(tempStr);
	items->appendChild(myType);

    xercesc::XMLString::transcode(fieldName.c_str(), tempStr, 99);
    xercesc::DOMNode* field = doc->createElement(tempStr);
    myType->appendChild(field);

    std::ostringstream stream;		// Can use boost to convert double to string more elegantly
    stream << data;
    std::string fieldValue = stream.str();

    xercesc::XMLString::transcode(fieldValue.c_str(), tempStr, 99);
    xercesc::DOMText* fieldvalue = doc->createTextNode(tempStr);
    field->appendChild(fieldvalue);

	xercesc::XMLString::transcode("utimestamp", tempStr, 99);
	xercesc::DOMNode* fieldTimestamp = doc->createElement(tempStr);
	myType->appendChild(fieldTimestamp);

	xercesc::XMLString::transcode("0", tempStr, 99);
	xercesc::DOMText* fieldTimestampValue = doc->createTextNode(tempStr);
	fieldTimestamp->appendChild(fieldTimestampValue);

    XMLOutput = serializeDOM(doc);

	doc->release();

    #ifdef XML_DEBUG
        std::cout << "XMLouput:" << std::endl << XMLOutput << std::endl;
    #endif
	// Other terminations and cleanup.
	return XMLOutput;

}	


/*
std::string XML::createNewInstallation(const std::string& nameValue, const std::string& descriptionValue, const std::string& inuseValue)
{
	XMLCh tempStr[100];

	std::string XMLOutput;

	std::cout << "begin of createNewInstallation" << std::endl;

	xercesc::XMLString::transcode("doc", tempStr, 99);
	xercesc::DOMDocument* doc = impl->createDocument(0, tempStr, 0);
	xercesc::DOMElement* docElement = doc->getDocumentElement();

	xercesc::XMLString::transcode("Installation", tempStr, 99);
	xercesc::DOMElement* installation = doc->createElement(tempStr);
	docElement->appendChild(installation);

	xercesc::XMLString::transcode("start", tempStr, 99);
	xercesc::DOMElement* start = doc->createElement(tempStr);
	installation->appendChild(start);

	xercesc::XMLString::transcode("2013-02-28T14:48:29", tempStr, 99);
	xercesc::DOMText* startFieldValue = doc->createTextNode(tempStr);
	start->appendChild(startFieldValue);

	xercesc::XMLString::transcode("end", tempStr, 99);
	xercesc::DOMElement* end = doc->createElement(tempStr);
	installation->appendChild(end);

	xercesc::XMLString::transcode("0001-01-01T00:00:00", tempStr, 99);
	xercesc::DOMText* endFieldValue = doc->createTextNode(tempStr);
	end->appendChild(endFieldValue);

	xercesc::XMLString::transcode("id", tempStr, 99);
	xercesc::DOMElement* id = doc->createElement(tempStr);
	installation->appendChild(id);

	xercesc::XMLString::transcode("0", tempStr, 99);
	xercesc::DOMText* idFieldValue = doc->createTextNode(tempStr);
	id->appendChild(idFieldValue);

	xercesc::XMLString::transcode("name", tempStr, 99);
	xercesc::DOMElement* name = doc->createElement(tempStr);
	installation->appendChild(name);

	xercesc::XMLString::transcode(nameValue.c_str(), tempStr, 99);
	xercesc::DOMText* nameFieldValue = doc->createTextNode(tempStr);
	name->appendChild(nameFieldValue);

	xercesc::XMLString::transcode("description", tempStr, 99);
	xercesc::DOMElement* description = doc->createElement(tempStr);
	installation->appendChild(description);

	xercesc::XMLString::transcode(descriptionValue.c_str(), tempStr, 99);
	xercesc::DOMText* descriptionFieldValue = doc->createTextNode(tempStr);
	description->appendChild(descriptionFieldValue);

	xercesc::XMLString::transcode("inuse", tempStr, 99);
	xercesc::DOMElement* inuse = doc->createElement(tempStr);
	installation->appendChild(inuse);

	xercesc::XMLString::transcode(inuseValue.c_str(), tempStr, 99);
	xercesc::DOMText* inuseFieldValue = doc->createTextNode(tempStr);
	inuse->appendChild(inuseFieldValue);

	xercesc::XMLString::transcode("infoname", tempStr, 99);
	xercesc::DOMElement* infoname = doc->createElement(tempStr);
	installation->appendChild(infoname);

	XMLOutput = serializeDOM(installation);
	doc->release();
	return XMLOutput;
}
*/

/*

std::string XML::createNewSensorGroup(const std::string& installationIDValue, const std::string& nameValue, const std::string& descriptionValue, const std::string& inuseValue)
{

	XMLCh tempStr[100];

	std::string XMLOutput;

	std::cout << "begin of createNEwSensorGroup" << std::endl;

	xercesc::XMLString::transcode("doc", tempStr, 99);
	xercesc::DOMDocument* doc = impl->createDocument(0, tempStr, 0);
	xercesc::DOMElement* docElement = doc->getDocumentElement();

	xercesc::XMLString::transcode("SensorGroup", tempStr, 99);
	xercesc::DOMElement* sensorGroup = doc->createElement(tempStr);
	docElement->appendChild(sensorGroup);

	xercesc::XMLString::transcode("start", tempStr, 99);
	xercesc::DOMElement* start = doc->createElement(tempStr);
	sensorGroup->appendChild(start);

	unsigned long int sec = time(NULL);
	std::cout << "time: " << std::endl << std::string(std::to_string(sec)) << std::endl;
	xercesc::XMLString::transcode(getCurrentTimestamp().c_str(), tempStr, 99);
	xercesc::DOMText* startFieldValue = doc->createTextNode(tempStr);
	start->appendChild(startFieldValue);

	xercesc::XMLString::transcode("end", tempStr, 99);
	xercesc::DOMElement* end = doc->createElement(tempStr);
	sensorGroup->appendChild(end);

	xercesc::XMLString::transcode("0001-01-01T00:00:00", tempStr, 99);
	xercesc::DOMText* endFieldValue = doc->createTextNode(tempStr);
	end->appendChild(endFieldValue);

	xercesc::XMLString::transcode("installationid", tempStr, 99);
	xercesc::DOMElement* installationid = doc->createElement(tempStr);
	sensorGroup->appendChild(installationid);

	xercesc::XMLString::transcode(installationIDValue.c_str(), tempStr, 99);
	xercesc::DOMText* installationidFieldValue = doc->createTextNode(tempStr);
	installationid->appendChild(installationidFieldValue);

	xercesc::XMLString::transcode("id", tempStr, 99);
	xercesc::DOMElement* id = doc->createElement(tempStr);
	sensorGroup->appendChild(id);

	xercesc::XMLString::transcode("0", tempStr, 99);
	xercesc::DOMText* idFieldValue = doc->createTextNode(tempStr);
	id->appendChild(idFieldValue);

	xercesc::XMLString::transcode("name", tempStr, 99);
	xercesc::DOMElement* name = doc->createElement(tempStr);
	sensorGroup->appendChild(name);

	xercesc::XMLString::transcode(nameValue.c_str(), tempStr, 99);
	xercesc::DOMText* nameFieldValue = doc->createTextNode(tempStr);
	name->appendChild(nameFieldValue);

	xercesc::XMLString::transcode("description", tempStr, 99);
	xercesc::DOMElement* description = doc->createElement(tempStr);
	sensorGroup->appendChild(description);

	xercesc::XMLString::transcode(descriptionValue.c_str(), tempStr, 99);
	xercesc::DOMText* descriptionFieldValue = doc->createTextNode(tempStr);
	description->appendChild(descriptionFieldValue);

	xercesc::XMLString::transcode("inuse", tempStr, 99);
	xercesc::DOMElement* inuse = doc->createElement(tempStr);
	sensorGroup->appendChild(inuse);

	xercesc::XMLString::transcode(inuseValue.c_str(), tempStr, 99);
	xercesc::DOMText* inuseFieldValue = doc->createTextNode(tempStr);
	inuse->appendChild(inuseFieldValue);

	xercesc::XMLString::transcode("infoname", tempStr, 99);
	xercesc::DOMElement* infoname = doc->createElement(tempStr);
	sensorGroup->appendChild(infoname);

	XMLOutput = serializeDOM(sensorGroup);
	doc->release();
	return XMLOutput;
}
*/

/*
std::string XML::createNewSensor(const std::string& sensorGroupIDValue, const std::string& nameValue, const std::string& dataNameValue, const std::string& descriptionValue, const std::string& inuseValue)
{

	XMLCh tempStr[100];

	std::string XMLOutput;

	std::cout << "begin of createNEwSensorGroup" << std::endl;

	xercesc::XMLString::transcode("doc", tempStr, 99);
	xercesc::DOMDocument* doc = impl->createDocument(0, tempStr, 0);
	xercesc::DOMElement* docElement = doc->getDocumentElement();

	xercesc::XMLString::transcode("Sensor", tempStr, 99);
	xercesc::DOMElement* sensor = doc->createElement(tempStr);
	docElement->appendChild(sensor);

	xercesc::XMLString::transcode("location", tempStr, 99);
	xercesc::DOMElement* location = doc->createElement(tempStr);
	sensor->appendChild(location);

	xercesc::XMLString::transcode("14.04", tempStr, 99);
	xercesc::DOMText* locationFieldValue = doc->createTextNode(tempStr);
	location->appendChild(locationFieldValue);

	xercesc::XMLString::transcode("sensorgroupid", tempStr, 99);
	xercesc::DOMElement* sensorgroupid = doc->createElement(tempStr);
	sensor->appendChild(sensorgroupid);

	xercesc::XMLString::transcode(sensorGroupIDValue.c_str(), tempStr, 99);
	xercesc::DOMText* sensorGroupFieldValue = doc->createTextNode(tempStr);
	sensorgroupid->appendChild(sensorGroupFieldValue);


	xercesc::XMLString::transcode("frequency", tempStr, 99);
	xercesc::DOMElement* frequency = doc->createElement(tempStr);
	sensor->appendChild(frequency);

	xercesc::XMLString::transcode("5", tempStr, 99);
	xercesc::DOMText* frequencyFieldValue = doc->createTextNode(tempStr);
	frequency->appendChild(frequencyFieldValue);

	xercesc::XMLString::transcode("dataname", tempStr, 99);
	xercesc::DOMElement* dataname = doc->createElement(tempStr);
	sensor->appendChild(dataname);

	xercesc::XMLString::transcode(dataNameValue.c_str(), tempStr, 99);
	xercesc::DOMText* datanameFieldValue = doc->createTextNode(tempStr);
	dataname->appendChild(datanameFieldValue);

	xercesc::XMLString::transcode("id", tempStr, 99);
	xercesc::DOMElement* id = doc->createElement(tempStr);
	sensor->appendChild(id);

	xercesc::XMLString::transcode("0", tempStr, 99);
	xercesc::DOMText* idFieldValue = doc->createTextNode(tempStr);
	id->appendChild(idFieldValue);

	xercesc::XMLString::transcode("name", tempStr, 99);
	xercesc::DOMElement* name = doc->createElement(tempStr);
	sensor->appendChild(name);

	xercesc::XMLString::transcode(nameValue.c_str(), tempStr, 99);
	xercesc::DOMText* nameFieldValue = doc->createTextNode(tempStr);
	name->appendChild(nameFieldValue);

	xercesc::XMLString::transcode("description", tempStr, 99);
	xercesc::DOMElement* description = doc->createElement(tempStr);
	sensor->appendChild(description);

	xercesc::XMLString::transcode(descriptionValue.c_str(), tempStr, 99);
	xercesc::DOMText* descriptionFieldValue = doc->createTextNode(tempStr);
	description->appendChild(descriptionFieldValue);

	xercesc::XMLString::transcode("inuse", tempStr, 99);
	xercesc::DOMElement* inuse = doc->createElement(tempStr);
	sensor->appendChild(inuse);

	xercesc::XMLString::transcode(inuseValue.c_str(), tempStr, 99);
	xercesc::DOMText* inuseFieldValue = doc->createTextNode(tempStr);
	inuse->appendChild(inuseFieldValue);

	xercesc::XMLString::transcode("infoname", tempStr, 99);
	xercesc::DOMElement* infoname = doc->createElement(tempStr);
	sensor->appendChild(infoname);

	XMLOutput = serializeDOM(sensor);
	doc->release();
	return XMLOutput;
}
*/

/*
std::string XML::createNewType(const std::string& aName, const std::vector<std::pair<std::string, std::string>>& aListOfFields) throw (UnknownDataType)
{
	std::string XMLOutput;
	XMLCh tempStr[100];
	std::cout << "begin of createNewType" << std::endl;

	xercesc::XMLString::transcode("doc", tempStr, 99);
	xercesc::DOMDocument* doc = impl->createDocument(0, tempStr, 0);
	xercesc::DOMElement* docElement = doc->getDocumentElement();

	xercesc::XMLString::transcode("usertype", tempStr, 99);
	xercesc::DOMElement* usertype = doc->createElement(tempStr);
	usertype->setAttribute(xercesc::XMLString::transcode("name"), xercesc::XMLString::transcode(aName.c_str()));
	docElement->appendChild(usertype);

	for(std::pair<std::string, std::string> aField : aListOfFields)
	{

		xercesc::XMLString::transcode("field", tempStr, 99);
		xercesc::DOMElement* field = doc->createElement(tempStr);
		usertype->appendChild(field);

		xercesc::XMLString::transcode("sql", tempStr, 99);
		xercesc::DOMElement* sql = doc->createElement(tempStr);
		field->appendChild(sql);

		xercesc::XMLString::transcode("type", tempStr, 99);
		xercesc::DOMElement* sqlType = doc->createElement(tempStr);
		sql->appendChild(sqlType);

		xercesc::XMLString::transcode(aField.second.c_str(), tempStr, 99);
		xercesc::DOMText* sqlTypeFieldValue = doc->createTextNode(tempStr);
		sqlType->appendChild(sqlTypeFieldValue);
	
		if(aField.second == std::string("decimal_ps_"))
		{
			xercesc::XMLString::transcode("l1", tempStr, 99);
			xercesc::DOMElement* l1 = doc->createElement(tempStr);
			sql->appendChild(l1);

			xercesc::XMLString::transcode("5", tempStr, 99);
			xercesc::DOMText* l1FieldValue = doc->createTextNode(tempStr);
			l1->appendChild(l1FieldValue);

			xercesc::XMLString::transcode("l2", tempStr, 99);
			xercesc::DOMElement* l2 = doc->createElement(tempStr);
			sql->appendChild(l2);

			xercesc::XMLString::transcode("2", tempStr, 99);
			xercesc::DOMText* l2FieldValue = doc->createTextNode(tempStr);
			l2->appendChild(l2FieldValue);

			xercesc::XMLString::transcode("type", tempStr, 99);
			xercesc::DOMElement* type = doc->createElement(tempStr);
			field->appendChild(type);

			xercesc::XMLString::transcode("double", tempStr, 99);
			xercesc::DOMText* typeFieldValue = doc->createTextNode(tempStr);
			type->appendChild(typeFieldValue);


		}
		else if(aField.second == std::string("float_n_"))
		{
			xercesc::XMLString::transcode("l1", tempStr, 99);
			xercesc::DOMElement* l1 = doc->createElement(tempStr);
			sql->appendChild(l1);

			xercesc::XMLString::transcode("5", tempStr, 99);
			xercesc::DOMText* l1FieldValue = doc->createTextNode(tempStr);
			l1->appendChild(l1FieldValue);

			xercesc::XMLString::transcode("type", tempStr, 99);
			xercesc::DOMElement* type = doc->createElement(tempStr);
			field->appendChild(type);

			xercesc::XMLString::transcode("float", tempStr, 99);
			xercesc::DOMText* typeFieldValue = doc->createTextNode(tempStr);
			type->appendChild(typeFieldValue);
		}
		else if(aField.second == std::string("char_n_"))
		{
			xercesc::XMLString::transcode("l1", tempStr, 99);
			xercesc::DOMElement* l1 = doc->createElement(tempStr);
			sql->appendChild(l1);

			xercesc::XMLString::transcode("255", tempStr, 99);
			xercesc::DOMText* l1FieldValue = doc->createTextNode(tempStr);
			l1->appendChild(l1FieldValue);

			xercesc::XMLString::transcode("type", tempStr, 99);
			xercesc::DOMElement* type = doc->createElement(tempStr);
			field->appendChild(type);

			xercesc::XMLString::transcode("", tempStr, 99);
			xercesc::DOMText* typeFieldValue = doc->createTextNode(tempStr);
			type->appendChild(typeFieldValue);
		}
		else if(aField.second == std::string("int_"))
		{
			xercesc::XMLString::transcode("type", tempStr, 99);
			xercesc::DOMElement* type = doc->createElement(tempStr);
			field->appendChild(type);

			xercesc::XMLString::transcode("int", tempStr, 99);
			xercesc::DOMText* typeFieldValue = doc->createTextNode(tempStr);
			type->appendChild(typeFieldValue);

		}
		else
		{
			throw UnknownDataType();
		}

		xercesc::XMLString::transcode("name", tempStr, 99);
		xercesc::DOMElement* name = doc->createElement(tempStr);
		field->appendChild(name);

		xercesc::XMLString::transcode(aField.first.c_str(), tempStr, 99);
		xercesc::DOMText* nameFieldValue = doc->createTextNode(tempStr);
		name->appendChild(nameFieldValue);

	}

	XMLOutput = serializeDOM(usertype);
	doc->release();

	return XMLOutput;
}
*/

std::string XML::login(const std::string& username, const std::string& password)
{
    #ifdef XML_DEBUG
        std::cout << "XML::login begin" << std::endl;
    #endif

    std::string XMLOutput;
	XMLCh tempStr[100];

	xercesc::XMLString::transcode("doc", tempStr, 99);
	xercesc::DOMDocument* doc = impl->createDocument();

	xercesc::XMLString::transcode("UserLogin", tempStr, 99);
	xercesc::DOMElement* userLoginNode = doc->createElement(tempStr);
	doc->appendChild(userLoginNode); 

	xercesc::XMLString::transcode("username", tempStr, 99);
	xercesc::DOMElement* usernameNode = doc->createElement(tempStr);
	userLoginNode->appendChild(usernameNode);

	xercesc::XMLString::transcode(username.c_str(), tempStr, 99);
	xercesc::DOMText* usernameValue = doc->createTextNode(tempStr);
	usernameNode->appendChild(usernameValue);

	xercesc::XMLString::transcode("password", tempStr, 99);
	xercesc::DOMElement* passwordNode = doc->createElement(tempStr);
	userLoginNode->appendChild(passwordNode);

	xercesc::XMLString::transcode(password.c_str(), tempStr, 99);
	xercesc::DOMText* passwordValue = doc->createTextNode(tempStr);
	passwordNode->appendChild(passwordValue);

	XMLOutput = serializeDOM(doc);


	doc->release();

	return XMLOutput;
}

xercesc::DOMDocument * XML::parseToDom(std::string data) throw(InvalidXMLError)
{
#ifdef XML_DEBUG
    std::cout << "XML::parseToDom(std::string data) begin" << std::endl;
#endif

	xercesc::DOMDocument *doc;

	xercesc::Wrapper4InputSource source(new xercesc::MemBufInputSource((const XMLByte*) (data.c_str()), data.size(), "100"));
    doc = parser->parse(&source);
	
	if (doc == NULL)
	{
		throw InvalidXMLError(); 
	}


#ifdef XML_DEBUG
    std::cout << "XML::parseToDom(std::string data) end" << std::endl;
#endif

	return doc;

}

std::string XML::analyzeLoginReply(const std::string& reply) throw (InvalidXMLError)
{
    #ifdef XML_DEBUG
        std::cout << "XML::analyzeLoginReply() begin" << std::endl;
    #endif

    std::string token;

    char * temp;

	xercesc::DOMDocument *doc;

    xercesc::DOMElement * docElement;

	doc = parseToDom(reply/*sting to parse to DOM*/); 
        doc->getDoctype();
    std::cout << "doc *: " << doc << std::endl;

    try{
        docElement = doc->getDocumentElement();
    }
        catch(...)
    {
        std::cerr << "catched" << std::endl;
    }
#ifdef XML_DEBUG
    std::cout << "XML::analyzeLoginReply() 2nd" << std::endl;
#endif

	xercesc::DOMElement * nextElement;
	nextElement = docElement->getFirstElementChild();
	while(nextElement != NULL)
	{
		XMLCh * tokentemp = xercesc::XMLString::transcode("token");
		XMLCh * errortemp = xercesc::XMLString::transcode("error");

		if(xercesc::XMLString::compareIString(nextElement->getTagName(), tokentemp) == 0)
		{
			temp = xercesc::XMLString::transcode(nextElement->getTextContent());
			token = std::string(temp);
			xercesc::XMLString::release(&temp);

		}
		else if(xercesc::XMLString::compareIString(nextElement->getTagName(), errortemp) == 0)
		{
			temp = xercesc::XMLString::transcode(nextElement->getTextContent());
			if(std::string(temp) == std::string("True"))
			{
				token = nullptr;
				throw IpsumError();	
			}
			xercesc::XMLString::release(&temp);
		}
		xercesc::XMLString::release(&tokentemp);
		xercesc::XMLString::release(&errortemp);

		nextElement = nextElement->getNextElementSibling();
	}	

    #ifdef XML_DEBUG
        std::cout << "token: " << token << std::endl;
    #endif

    //doc->release();
    //docElement->release();
    #ifdef XML_DEBUG
	std::cout << "XML::analyzeLoginReply() end token: "  <<std::endl;
    #endif

	return token;

}



std::string XML::selectData(const std::vector<std::string>& fields, const std::string& startTime, const std::string& endTime)
{
	std::string XMLOutput;

	XMLCh tempStr[100];

	xercesc::XMLString::transcode("doc", tempStr, 99);
	xercesc::DOMDocument* doc = impl->createDocument();

	xercesc::XMLString::transcode("get", tempStr, 99);
	xercesc::DOMElement* getNode = doc->createElement(tempStr);
	doc->appendChild(getNode); 

	xercesc::XMLString::transcode("start", tempStr, 99);
	xercesc::DOMElement* startNode = doc->createElement(tempStr);
	getNode->appendChild(startNode);


	xercesc::XMLString::transcode(startTime.c_str(), tempStr, 99);
	xercesc::DOMText* startNodeValue = doc->createTextNode(tempStr);
	startNode->appendChild(startNodeValue);

	xercesc::XMLString::transcode("end", tempStr, 99);
	xercesc::DOMElement* endNode = doc->createElement(tempStr);
	getNode->appendChild(endNode);

	xercesc::XMLString::transcode(endTime.c_str(), tempStr, 99);
	xercesc::DOMText* endNodeValue = doc->createTextNode(tempStr);
	endNode->appendChild(endNodeValue);

	xercesc::XMLString::transcode("select", tempStr, 99);
	xercesc::DOMElement* selectNode = doc->createElement(tempStr);
	getNode->appendChild(selectNode);

	

	for(auto it = fields.begin(); it < fields.end(); ++it)
	{
		xercesc::XMLString::transcode("field", tempStr, 99);
		xercesc::DOMElement* fieldNode = doc->createElement(tempStr);
		selectNode->appendChild(fieldNode);
		
		xercesc::XMLString::transcode("name", tempStr, 99);
		xercesc::DOMElement* nameNode = doc->createElement(tempStr);
		fieldNode->appendChild(nameNode);

		xercesc::XMLString::transcode((*it).c_str(), tempStr, 99);
		xercesc::DOMText* fieldValue = doc->createTextNode(tempStr);
		nameNode->appendChild(fieldValue);
	}


	XMLOutput = serializeDOM(doc);
	doc->release();


	return XMLOutput;
}

std::vector<std::pair<std::string, std::string>> analyzeSelect(std::string input)
{
	std::vector<std::pair<std::string, std::string>> output;
	
	
	return output;
}

std::string XML::getTimestamp(int houres, int minutes, int seconds, int day, int month, int year)
{
	
	boost::gregorian::date date(year, month, day);
	boost::posix_time::time_duration time(houres, minutes, seconds);
	
	
	boost::posix_time::ptime t(date, time);

	return boost::posix_time::to_iso_extended_string(t);

}

std::string XML::getCurrentTimestamp()
{
	// formate: 2013-03-03T18:28:02
	boost::posix_time::ptime t(boost::posix_time::second_clock::universal_time());

    #ifdef XML_DEBUG
        std::cout << "boost time" << boost::posix_time::to_iso_extended_string(t) << std::endl;
    #endif

	return boost::posix_time::to_iso_extended_string(t);
}

std::string XML::getTimeInSeconds()
{
	unsigned long int sec = time(NULL);
	return std::string(std::to_string(sec));
}
