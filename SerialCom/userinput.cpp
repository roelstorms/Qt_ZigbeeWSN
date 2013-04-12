#include "userinput.h"


UserInput::UserInput(bool * a_stop) : stop(a_stop), inputMode(NONE)
{
	std::cout << "stop in Userinput::UserInput: " << stop << std::endl;
}

UserInput::UserInput(const UserInput & source)
{
	stop = source.stop;
	inputMode = source. inputMode;
	std::cout << "stop in Userinput::UserInput CC: " << stop << std::endl;
}

UserInput::~UserInput()
{

}


void UserInput::operator() ()
{
	std::string input;
	std::cout << "UserInput::operator() ()" << std::endl;
	std::cout << "stop bool in UserInput: " << *stop << std::endl;
	while(input != std::string("quit"))
	{
		std::cin >> input;
		if(input == std::string("send"))
		{
			std::system("clear");
			std::cout << "Send mode" << std::endl;
			inputMode = SEND;
		}
		std::cout << "input: " << input << std::endl;
	}
	std::cout << "stop set to true" << std::endl;
	*stop = true;
}
