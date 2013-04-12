/*
	
   */


#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "./HTTP/http.h"
#include "./XML/XML.h"

class DataHandler
{
	public:
	       	bool operator() (float data){			
			Http socket(std::string(""));
			socket.uploadData(data);
		}
};

#endif
