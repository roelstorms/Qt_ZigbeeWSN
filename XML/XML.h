/*
 *  Created by Roel Storms
 *
 *  A lot of XML functionality can be found in this class. A number of functions
 *  are no longer used and should be tested before using them again.
 *
 *  This class contains functions to create XML for Ipsum requests or analyze
 *  XML coming from a login request to Ipsum. This class is mostly used in the HTTP
 *  class.
 *
 *  Some XML functionality can also be found in the WSPackets.
 *
 */

#ifndef XML_H
#define XML_H

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

#include <string>
#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>

#include "../errors.h"
#include "../enums.h"

class XML
{
	private:
	xercesc::DOMImplementation *impl; 
    xercesc::DOMLSParser *parser;
	public:
    XML() throw (XercesError);
	~XML();
    std::string serializeDOM(xercesc::DOMNode * node);

	std::string uploadData(const std::string& type, const std::vector<std::pair<std::string, double>>& input);
	std::string uploadData(const std::string& type, const std::string& fieldName, float data, std::string timeStamp); 
	

    xercesc::DOMDocument * parseToDom(std::string data) throw(InvalidXMLError);

    //std::string createNewInstallation(const std::string& nameValue, const std::string& descriptionValue, const std::string& inuseValue);
    //std::string createNewSensorGroup(const std::string& installationIDValue, const std::string& nameValue, const std::string& descriptionValue, const std::string& inuseValue);
    //std::string createNewSensor(const std::string& sensorGroupIDValue, const std::string& nameValue, const std::string& dataNameValue, const std::string& descriptionValue, const std::string& inuseValue);

	/*
	 *	Creates XML that generates a new type on the ipsum server. 
	 *	You can specify the name of this new type and several different field. 
	 *	Each field has a name and a type (for instance float_n or int_ or text (see ipsum documentation for the different types possible)
	 *
	 */
    //std::string createNewType(const std::string& aName, const std::vector<std::pair<std::string, std::string>>& aListOfFields) throw (UnknownDataType);


	std::string login(const std::string& username, const std::string& password);

	/*
	 *	Reply is filename where the XML returned from ipsum on a login request is stored.
	 *	Function returns token needed by ipsum for requests which required to be logged in.
	 */
    std::string analyzeLoginReply(const std::string& reply)  throw (InvalidXMLError);

	/*
	 *	Generates XML needed by the ipsum POST request to do a select.
	 *	No possibility to apply functions or name as or where which are all supported by ipsum.
	 *	TODO: test new implementation 
	 */
	std::string selectData(const std::vector<std::string>& fields, const std::string& startTime, const std::string& endTime);

	/*
	 *	analyzeSelect analyzes the ipsum XML response on a select call.
	 *	Returns a list of pairs where pair.first is the fieldname and pair.second is the value
	 *
	 */
	std::vector<std::pair<std::string, std::string>> analyzeSelect(const std::string& input);

	/*
	 * 	getTimestamp returns a timestamp in the format YYYY-DD-MMThh:mm:ss
	 */
	std::string getTimestamp(int houres, int minutes, int seconds, int day, int month, int year);

	/*
	 *	Does the same as getTimestamp but for the current time
	 */
	std::string getCurrentTimestamp();

	/*
	 *	Get seconds since 1 Jan 1970 0:00-UTC
	 */
	std::string getTimeInSeconds();
};


#endif
