#include <typeinfo>
#include <iostream>

class Base
{
	public :
		Base(){ std::cout << "base constructor" << std::endl;};
		virtual ~Base(){};
		virtual void getType(){std::cout << " Base gettype" << std::endl;};
};

class Derived : public Base
{
	public :
		Derived(){ std::cout << "derived constructor" << std::endl;};
		virtual ~Derived(){};
		virtual void  getType(){std::cout << " Derived gettype" << std::endl;};
};


int main(int argc, char ** argv)
{
	Base base;
	Base * basep;
	basep = &base;
	Derived derived;
	Derived * derivedp = &derived;

	std::cout << "base type: " << typeid(base).name() << std::endl;
	std::cout << "base* type: " << typeid(basep).name() << std::endl;
	std::cout << "derived type: " << typeid(derived).name() << std::endl;
	std::cout << "derived* type: " << typeid(derivedp).name() << std::endl;

	std::cout << "dynamic cast of derived to Derived* "  << dynamic_cast<Derived*> (derivedp) << std::endl;
	basep = (derivedp);

	std::cout << "derivedp in basep: " << typeid(basep).name() << std::endl;

	std::cout << "dynamic cast of derived to Derived* "  << dynamic_cast<Derived*> (derivedp) << std::endl;

	std::cout << "dynamic cast of basep to Derived* "  << dynamic_cast<Derived*> (basep) << std::endl;
	basep = dynamic_cast<Base*> (new Derived());
	std::cout << "dynamic cast of basep to Derived* "  << dynamic_cast<Derived*> (basep) << std::endl;
	basep->getType();

}
