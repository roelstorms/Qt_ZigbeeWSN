CC = g++
LFLAGS = -Wall -Werror -std=c++11 -g -lpthread  
CFlAGS = -Wall -Werror -std=c++11 -g -DDEBUG
LIBS = -ldl -lcurl -lxerces-c -lboost_thread -lboost_date_time
#SOURCES = ZBReceiver.cpp
#DEPENDENCIES = $(SOURCES:.cpp=.h)
#OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS = http.o ipsum.o ipsumpacket.o ipsumuploadpacket.o ipsumchangeinusepacket.o ipsumchangefreqpacket.o

RM = /bin/rm -f

all : $(OBJECTS)
http_test.out : http_test.cpp $(OBJECTS) 
	$(CC)  $(LFLAGS) http_test.cpp -o http_test.out ../XML/XML.o ../packet.o ../packetqueue.o $(OBJECTS) $(LIBS) 

 
$(OBJECTS): %.o:  %.cpp %.h
	$(CC) -c $(LFLAGS) $< -o $@


clean:
	$(RM) $(OBJECTS)
