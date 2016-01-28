#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <ctime>

#include <sfml\Network.hpp>

#include <stdlib.h>
#include <crtdbg.h>
#include "Core\Core.h"

#include <vld.h>

using namespace std;

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	double dt = 0.0f;
	float timePass = 0.0f;
	int fps = 0;
	unsigned int start = clock();

	srand(unsigned int(time(0)));

	Core core;
	core.init();

	while (core.windowVisible())
	{
		//sleep(milliseconds(24));
		//dt = 0.100;

		if (dt > 0.1)
			dt = 0.1;

		core.update(float(dt));

		unsigned int temp = clock();
		dt = unsigned int(temp - start) / double(1000);
		timePass += float(dt);
		start = temp;
		fps++;

		if (timePass > 1.0f)
		{
			core.setfps(fps);
			timePass = 0.0f; //timePass -= 1.0f;
			fps = 0;
		}
	}
};