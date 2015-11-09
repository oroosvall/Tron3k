#include "Engine.h"
#include <iostream>

void test()
{
	Engine* e = new Engine();
	std::cout << "hej!" << std::endl;
	delete e;
}