#include "http.h"


Http::Http(std::string urlBase, std::string PersonalKey): urlBase(urlBase), PersonalKey(PersonalKey)
{
    #ifdef HTTP_DEBUG
	std::cout << "Http constructor" << std::endl;
    #endif

	curl_global_init(CURL_GLOBAL_ALL);
	token = std::string();
	httpError = -1;
}

Http::~Http()
{
	curl_global_cleanup();

    #ifdef HTTP_DEBUG
	std::cout << "Http destructor" << std::endl;
    #endif
}

size_t Http::standardReplyWrapper(void *buffer, size_t size, size_t nmemb, void *obj)
{
    //std::cout << "standardReplyWrapper" << std::endl;
	return static_cast<Http*>(obj)->write_data(buffer, size, nmemb);
}

size_t Http::write_data(void *buffer, size_t size, size_t nmemb)
{
    //std::cout << "write_data" << std::endl;
	curlReply = std::string((char *)buffer);

	return size * nmemb;
}

size_t Http::headerHandlerWrapper(void *buffer, size_t size, size_t nmemb, void *obj)
{
	return static_cast<Http*>(obj)->headerHandler(buffer, size, nmemb);
}

size_t Http::headerHandler(void *buffer, size_t size, size_t nmemb)
{
	std::string header((char *) buffer);
	if(header.find(std::string("HTTP/1.1 ")) != std::string::npos)
	{
		httpError = boost::lexical_cast<int>(header.substr(header.find(std::string("HTTP/1.1 ")) + 9, 3));
	}
	return size * nmemb;
}



std::string Http::sendGet(std::string urlAddition, size_t (*callback) (void*, size_t, size_t, void*)) throw (HttpError)
{
	curl = curl_easy_init();
    #ifdef HTTP_DEBUG
	std::cout << "sendGet" << std::endl;
    #endif
	CURLcode result;
	std::string url(urlBase);
	url.append(urlAddition);	
	std::cout << "string used:" << std::endl << url << std::endl << std::endl;
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		
		curl_easy_setopt(curl, CURLOPT_HEADER, 1);			// Enables the output of header information
		curl_easy_setopt(curl, CURLOPT_WRITEHEADER, this);		// Give this as a paramater to the HEADERFUNCTION
 		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerHandlerWrapper);		// set headerHandlerWrapper as a callbackfunction to parse header information

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback); 
		/* Perform the request, res will get the return code */
		result = curl_easy_perform(curl);
		/* Check for errors */
		if(result != CURLE_OK)
		{
            std::cout << "curl_easy_perform() failed: " <<  curl_easy_strerror(result) << std::endl;
			throw HttpError();
		}		
		/* always cleanup */
	}
	curl_easy_cleanup(curl);
	return curlReply;
}

std::string Http::sendPost(std::string urlAddition, std::string data, size_t (*callback) (void *, size_t, size_t, void *)) throw (HttpError)
{
	curl = curl_easy_init();
    #ifdef HTTP_DEBUG
	std::cout << "sendPost" << std::endl;
    #endif
	CURLcode result;
	std::string url(urlBase);
	url.append(urlAddition);	
    #ifdef HTTP_DEBUG
	std::cout << "string used:" << std::endl << url << std::endl << std::endl;
	std::cout << "data sent: " << std::endl << data << std::endl << std::endl;
    #endif
    if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		
		curl_easy_setopt(curl, CURLOPT_HEADER, 1);			// Enables the output of header information
		curl_easy_setopt(curl, CURLOPT_WRITEHEADER, this);		// Give this as a paramater to the HEADERFUNCTION
 		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerHandlerWrapper);		// set headerHandlerWrapper as a callbackfunction to parse header information

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

		/* Perform the request, res will get the return code */

		result = curl_easy_perform(curl);
		/* Check for errors */
		if(result != CURLE_OK)
		{	
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			throw HttpError();
		}
		/* always cleanup */
	}
    #ifdef HTTP_DEBUG
	std::cout << "end of sendpost" << std::endl;
    #endif
    curl_easy_cleanup(curl);
	if(httpError != 200)
	{
        throw HttpError();
	}
	return curlReply;

}
std::string Http::generateCode(std::string url)
{
    #ifdef HTTP_DEBUG
	std::cout << std::endl << "generating code from url: " << std::endl << url << std::endl << std::endl;
    #endif
	const unsigned char * input = (const unsigned char*) url.c_str();
	unsigned char * output =  (unsigned char *) malloc(sizeof(unsigned char) * 20);
	SHA1(input, url.size(), output);
	std::string code;
	char temp[20];
	for (int i = 0; i < 20; i++) 
	{
		sprintf(temp, "%02x", output[i]);
		code.append(temp);	

	}
    #ifdef HTTP_DEBUG
	std::cout << std::endl << "code: " << code << std::endl;
    #endif
	delete output;
	return code;
}


std::string Http::calculateDestination(int userID, int installationID, int sensorGroupID, int sensorID)
{
	std::string output;
	output.append(std::to_string(userID));

	int checksum = userID;

	if(installationID != -1)
	{
		output.append(":");
		output.append(std::to_string(installationID));
		checksum += installationID;
	}

	if(sensorGroupID != -1)
	{
		output.append(":");
		output.append(std::to_string(sensorGroupID));
		checksum += sensorGroupID;
	}

	if(sensorID != -1)
	{
		output.append(":");
		output.append(std::to_string(sensorID));
		checksum += sensorID;
	}
	output.append(":");

	unsigned long int sec = time(NULL);
	std::cout << "time: " << std::endl << std::string(std::to_string(sec)) << std::endl;
	checksum += sec;
	output.append(std::to_string(sec));
	output.append(":");

	std::string stringChecksum(std::to_string(checksum));
    #ifdef HTTP_DEBUG_2
	std::cout << "long checksum:" << std::endl << stringChecksum << std::endl;
    #endif
	std::string shortChecksum(stringChecksum.end()-6, stringChecksum.end());
    #ifdef HTTP_DEBUG_2
	std::cout << "short checksum" << std::endl << shortChecksum << std::endl;
    #endif
	output.append(shortChecksum);

	std::string output2 = toBase64(output);
	return output2;
}


std::string Http::toBase64(std::string input)
{
	using namespace boost::archive::iterators;


    typedef transform_width< binary_from_base64<remove_whitespace<std::string::const_iterator> >, 8, 6 > it_binary_t;
    typedef insert_linebreaks<base64_from_binary<transform_width<std::string::const_iterator,6,8> >, 72 > it_base64_t;

    #ifdef HTTP_DEBUG_2
    std::cout << "Base 64 string is: '" << input << "'" << std::endl;
    #endif

	// Encode
	unsigned int writePaddChars = (3-input.length()%3)%3;
    std::string base64(it_base64_t(input.begin()),it_base64_t(input.end()));
	base64.append(writePaddChars,'=');

    #ifdef HTTP_DEBUG_2
    std::cout << "Base64 representation: " << std::base64 << std::endl;
    #endif
	return base64;
}


void Http::uploadData(IpsumUploadPacket * packet) throw (HttpError)
{
	std::string url;
	std::string temp;

    #ifdef HTTP_DEBUG
	std::cout << "Http::uploadData" << std::endl;
    #endif

	login();
	std::string timeStamp = xmlParser.getTimeInSeconds();
	auto data = packet->getData();
	for(auto it = data.begin(); it < data.end(); ++it)
	{
		url.clear();
		url.append("/upload");
		url.append("/");
		url.append(calculateDestination(21, packet->getInstallationID(), packet->getSensorGroupID(), std::get<1>(*it)));
		url.append("/");
		temp.clear();
		temp.append(url);
        temp.append(PersonalKey);
		url.append(generateCode(temp));
		std::string sensorType;
		std::string fieldName;	
        #ifdef HTTP_DEBUG_2
        std::cout << "sensor Type to be uploaded: " << std::get<0>(*it) << std::endl;
        #endif
        switch(std::get<0>(*it))
		{
			case TEMP:
				sensorType = "zigbeeTemp";
				fieldName = "temperature";
			break;
			case HUM:
				sensorType = "zigbeeHum";
				fieldName = "humidity";
			break;
			case PRES:
				sensorType = "zigbeePres";
				fieldName = "pressure";
			break;
			case BAT:
				sensorType = "zigbeeBat";
				fieldName = "battery";
			break;
			case CO2:
				sensorType = "zigbeeCO2";
				fieldName = "CO2";
			break;
			case ANEMO:
				sensorType = "zigbeeAnemo";
				fieldName = "anemo";
			break;
			case VANE:
				sensorType = "zigbeeVane";
				fieldName = "vane";
			break;
			case PLUVIO:
				sensorType = "zigbeePluvio";
				fieldName = "pluvio";
			break;
		}

        sendPost(url, xmlParser.uploadData(sensorType, "value", std::get<2>(*it), timeStamp), &Http::standardReplyWrapper);
        #ifdef HTTP_DEBUG
        std::cout << fieldName << " data uploaded to ipsum" << std::endl;
        #endif
    }
	
}

void Http::uploadData(std::string aSensorType, std::string destinationBase64, std::vector<std::pair<std::string, double>> input) throw (HttpError)
{
	std::string url;
	std::string temp;

    #ifdef HTTP_DEBUG
	std::cout << "Http::uploadData" << std::endl;
    #endif

	login();

	url.clear();
	url.append("/upload");
	url.append("/");
	url.append(destinationBase64);
	url.append("/");
	temp.clear();
	temp.append(url);
    temp.append(PersonalKey);
	url.append(generateCode(temp));
	
	XML XMLParser;
	sendPost(url, XMLParser.uploadData(aSensorType, input), &Http::standardReplyWrapper);
}

bool Http::login() throw (HttpError, InvalidLogin)
{
	if(token.empty() || tokenExpireTime <= boost::posix_time::second_clock::universal_time())
	{
		std::string url("/auth/");
		std::string temp;
		temp.append(url);
        url.append(generateCode(temp.append(PersonalKey)));

        XML XMLParser;

        httpError = -1;

		std::string loginReply = sendPost(url, XMLParser.login(std::string("roel"), std::string("roel")), &Http::standardReplyWrapper);
		if(httpError != 200)
		{
			throw HttpError();
		}

		token = XMLParser.analyzeLoginReply(loginReply);
        #ifdef HTTP_DEBUG_2
		std::cout << "token calculated from login reply:  " << token << std::endl;
        #endif
		if (token.empty())
		{
			std::cerr << "Login failed, reply contained error = true" << std::endl;
			throw InvalidLogin();
			return false;
		}
		else
		{
			tokenExpireTime = boost::posix_time::ptime (boost::posix_time::second_clock::universal_time() + boost::posix_time::minutes(30));
            #ifdef HTTP_DEBUG
			std::cout << "tokenExpireTime: " << boost::posix_time::to_simple_string(tokenExpireTime) << std::endl;
            #endif
        }
	}
	return true;
}
void Http::setUserRights(std::string entity, int userID, int rights) throw (HttpError)
{
    #ifdef HTTP_DEBUG
	std::cout << "Http::setUserRights" << std::endl;
    #endif

	login();
	
	std::string url;
	std::string temp;
	
	// url format for entity: entity/{token}/{destination}/{code}
	url.clear();
	url.append("/setUser/");
	url.append(std::to_string(userID) );
	url.append(":");
	url.append(std::to_string(rights));
	url.append("/");
	url.append(token);		
	url.append("/");
	temp.clear();
	temp.append(url);
    temp.append(PersonalKey);
    url.append(generateCode(temp));
    sendPost(url, entity, &Http::standardReplyWrapper);

}

std::string Http::getEntity(std::string destinationBase64) throw (HttpError)
{

	std::cout << "Http::getEntity" << std::endl;
	XML XMLParser;

	login();
	
	std::string url;
	std::string temp;
	
	// url format for entity: entity/{token}/{destination}/{code}
	url.clear();
	url.append("/entity/");
	url.append(token);		
	url.append("/");
	url.append(destinationBase64);
	url.append("/");
	temp.clear();
	temp.append(url);
    temp.append(PersonalKey);
	url.append(generateCode(temp));
	return sendGet(url, &Http::standardReplyWrapper);
	

}

std::string Http::getChildren(std::string destinationBase64) throw (HttpError)
{
    #ifdef HTTP_DEBUG
	std::cout << "Http::getEntity" << std::endl;
    #endif
	XML XMLParser;

	login();
	
	std::string url;
	std::string temp;
	
	// url format for entity: entity/{token}/{destination}/{code}
	url.clear();
	url.append("/children/");
	url.append(token);		
	url.append("/");
	url.append(destinationBase64);
	url.append("/");
	temp.clear();
	temp.append(url);
    temp.append(PersonalKey);
	url.append(generateCode(temp));
	return sendGet(url, &Http::standardReplyWrapper);
}

void Http::setToken(std::string token)
{
    this->token = token;
}

std::string Http::selectData(std::string destinationBase64, std::vector<std::string> fields) throw (HttpError)
{
    #ifdef HTTP_DEBUG
	std::cout << std::endl << "Http::selectData" << std::endl << std::endl;
    #endif

	XML XMLParser;

	login();

	std::string url;
	std::string temp;
	
	// url format for entity: select/{token}/{destination}/{code}
	url.clear();
	url.append("/select/");
	url.append(token);		
	url.append("/");
	
	url.append(destinationBase64);

	url.append("/");
	temp.clear();
	temp.append(url);
    temp.append(PersonalKey);
    url.append(generateCode(temp));


    return sendPost(url, XMLParser.selectData(fields,  XMLParser.getTimestamp(1, 0, 0, 1, 3, 2013), XMLParser.getCurrentTimestamp()), &Http::standardReplyWrapper);
	//sendPost(url, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<get>\n<start>2012-01-01T00:00:00</start>\n<end>9999-12-31T23:59:59</end>\n<select>\n<field>\n<name>intensity</name>\n</field>\n</select>\n</get>", &Http::standardReplyWrapper);
}



std::string Http::ipsumInfo() throw (HttpError)
{
	std::string url;
	url.append("/info");

	return sendGet(url, &Http::standardReplyWrapper);
}

/*
std::string Http::createNewSensor(std::string sensorGroupIDValue, std::string nameValue, std::string dataNameValue, std::string descriptionValue, std::string inuseValue) throw (HttpError)
{
	XML XMLParser;
	
	login();

	std::string url;
	std::string temp;
	
	// url format for addSensor:  addSensor/{token}/{code}
	url.clear();
	url.append("/addSensor/");
	url.append(token);		
	url.append("/");
	temp.clear();
	temp.append(url);
    temp.append(PersonalKey);
	url.append(generateCode(temp));

    return sendPost(url, XMLParser.createNewSensor(sensorGroupIDValue, nameValue, dataNameValue, descriptionValue, inuseValue), &Http::standardReplyWrapper);
}
*/

/*
std::string Http::createNewType(std::string aName, std::vector<std::pair<std::string, std::string>> aListOfFields) throw (HttpError)

{
	XML XMLParser;
	
	login();

	std::string url;
	std::string temp;
	
	// url format for addSensor:  addSensor/{token}/{code}
	url.clear();
	url.append("/addType/");
	url.append(token);		
	url.append("/");
	
	temp.clear();
	temp.append(url);
    temp.append(PersonalKey);
	url.append(generateCode(temp));

	return sendPost(url, XMLParser.createNewType(aName, aListOfFields), &Http::standardReplyWrapper);
}
*/

std::string Http::changeSensorGroup(std::string newXML)
{
    XML XMLParser;

    login();

    std::string url;
    std::string temp;

    // url format for addGroup:  addGroup/{token}/{code}
    url.clear();
    url.append("/addGroup/");
    url.append(token);
    url.append("/");

    temp.clear();
    temp.append(url);
    temp.append(PersonalKey);
    url.append(generateCode(temp));

    return sendPost(url, newXML, &Http::standardReplyWrapper);
}

std::string Http::changeSensor(std::string newXML)
{
    XML XMLParser;

    login();

    std::string url;
    std::string temp;

    // url format for addSensor:  addSensor/{token}/{code}
    url.clear();
    url.append("/addSensor/");
    url.append(token);
    url.append("/");

    temp.clear();
    temp.append(url);
    temp.append(PersonalKey);
    url.append(generateCode(temp));

    return sendPost(url, newXML, &Http::standardReplyWrapper);
}

void Http::changeInUse(IpsumChangeInUsePacket * packet) throw(HttpError)
{
    /*
     *  Changing in use of the sensorGroup
     */
    #ifdef HTTP_DEBUG
    std::cout << "Http::changeInUse()" << std::endl;
    #endif
	std::string entity = getEntity(calculateDestination(21 ,packet->getInstallationID(), packet->getSensorGroupID()));
	XML XMLParser;
	xercesc::DOMDocument * doc= XMLParser.parseToDom(entity);
	xercesc::DOMElement * docElement = doc->getDocumentElement();
		
	xercesc::DOMElement * nextElement;
	nextElement = docElement->getFirstElementChild();
	while(nextElement != NULL)
	{
		XMLCh * inusetemp = xercesc::XMLString::transcode("inuse");

		if(xercesc::XMLString::compareIString(nextElement->getTagName(), inusetemp) == 0)
		{
			XMLCh * inUse;
			if(packet->getInUse())
			{
				inUse = xercesc::XMLString::transcode("True");
			}
			else
			{

				inUse = xercesc::XMLString::transcode("False");
			}
			nextElement->setTextContent(inUse);
            xercesc::XMLString::release(&inUse);

		}

		xercesc::XMLString::release(&inusetemp);

		nextElement = nextElement->getNextElementSibling();
	}	
    changeSensorGroup(XMLParser.serializeDOM(doc));


    /*
     * Changing in use of all sensors
     */
    std::map<int, bool> sensors = packet->getSensors();
    for(auto sensorsIt = sensors.begin(); sensorsIt != sensors.end(); ++sensorsIt)
    {
        std::string entity = getEntity(calculateDestination(21 ,packet->getInstallationID(), packet->getSensorGroupID(), sensorsIt->first));
        XML XMLParser;
        xercesc::DOMDocument * doc= XMLParser.parseToDom(entity);
        xercesc::DOMElement * docElement = doc->getDocumentElement();

        xercesc::DOMElement * nextElement;
        nextElement = docElement->getFirstElementChild();
        while(nextElement != NULL)
        {
            XMLCh * inusetemp = xercesc::XMLString::transcode("inuse");

            if(xercesc::XMLString::compareIString(nextElement->getTagName(), inusetemp) == 0)
            {
                XMLCh * inUse;
                if(sensorsIt->second)
                {
                    inUse = xercesc::XMLString::transcode("True");
                }
                else
                {

                    inUse = xercesc::XMLString::transcode("False");
                }
                nextElement->setTextContent(inUse);
                xercesc::XMLString::release(&inUse);

            }

            xercesc::XMLString::release(&inusetemp);

            nextElement = nextElement->getNextElementSibling();
        }
        changeSensor(XMLParser.serializeDOM(doc));
    }
}

void Http::changeFreq(IpsumChangeFreqPacket *packet)  throw (HttpError)
{
    #ifdef HTTP_DEBUG
    std::cout << "Http::changeFreq()" << std::endl;
    #endif

    int installationID = packet->getInstallationID();
    int sensorGroupID = packet->getSensorGroupID();
    std::vector<std::pair<int, int> > frequencies = packet->getFrequencies();
    for(auto it = frequencies.begin(); it < frequencies.end(); ++it)
    {
        std::string entity = getEntity(calculateDestination(21 , installationID, sensorGroupID, it->first));

        XML XMLParser;
        xercesc::DOMDocument * doc= XMLParser.parseToDom(entity);
        xercesc::DOMElement * docElement = doc->getDocumentElement();

        xercesc::DOMElement * nextElement;
        nextElement = docElement->getFirstElementChild();
        while(nextElement != NULL)
        {
            XMLCh * frequencytemp = xercesc::XMLString::transcode("frequency");

            if(xercesc::XMLString::compareIString(nextElement->getTagName(), frequencytemp) == 0)
            {
                XMLCh * freq;

                freq = xercesc::XMLString::transcode(std::to_string(it->second).c_str());

                nextElement->setTextContent(freq);
                xercesc::XMLString::release(&freq);

            }

            xercesc::XMLString::release(&frequencytemp);

            nextElement = nextElement->getNextElementSibling();
        }
        changeSensor(XMLParser.serializeDOM(doc));
    }
}


/*
std::string Http::createNewSensorGroup(const std::string& installationIDValue, const std::string& nameValue, const std::string& descriptionValue, const std::string& inuseValue)
{
	XML XMLParser;
	
	login();

	std::string url;
	std::string temp;
	
	// url format for entity: select/{token}/{destination}/{code}
	url.clear();
	url.append("/addGroup/");
	url.append(token);		
	url.append("/");
	
	temp.clear();
	temp.append(url);
    temp.append(PersonalKey);
	url.append(generateCode(temp));

	return sendPost(url, XMLParser.createNewSensor(nstallationIDValue, nameValue, descriptionValue, inuseValue);
, &Http::standardReplyWrapper);



}
*/
