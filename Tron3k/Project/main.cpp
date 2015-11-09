#include <iostream>
#include <ctime>

#include <sfml\Network.hpp>

#include "Core\Core.h"

using namespace std;

int main()
{
	double dt = 0.0f;
	float timePass = 0.0f;
	int fps = 0;
	unsigned int start = clock();

	Core core;
	core.init();

	while (true)
	{
		core.update(dt);

		unsigned int temp = clock();
		dt = unsigned int(temp - start) / double(1000);
		timePass += dt;
		start = temp;
		fps++;

		if (timePass > 1.0f)
		{
			timePass = 0.0f; //timePass -= 1.0f;
			fps = 0;
		}
	}
};