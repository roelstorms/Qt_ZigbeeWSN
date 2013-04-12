#ifndef USERINPUT_H
#define USERINPUT_H

#include <string>
#include <boost/thread.hpp>
#include <stdio.h>   /* Standard input/output definitions */
#include <iostream>
#include <fstream>
#include <stdlib.h>

enum Mode
{
	NONE, SEND, RECEIVE
};

class UserInput
{
	private:
	bool * stop;
	Mode inputMode;
	public:
	UserInput(bool * a_stop);
	~UserInput();
	UserInput(const UserInput & source);
	void operator () ();
};

#endif
