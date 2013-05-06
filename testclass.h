#ifndef TESTCLASS_H
#define TESTCLASS_H
#include <iostream>
#include <string>
class TestClass
{
	private:

		TestClass();
	public:
        static void assertTest(bool condition, std::string message)
		{
			if(condition)
			{
				std::cout << message << "\033[1;32m succes\033[0m" << std::endl;
			}
			else
			{
				std::cout << message << "\033[1;31m failed\033[0m" << std::endl;
			}
		};
};


#endif
