#include <iostream>
#include <ctime>

#include "Network\Client.h"
#include "Network\Server.h"

using namespace std;

int main()
{
	cout << "[1] Client" << endl;
	cout << "[2] Server" << endl;
	cout << "[3] Exit" << endl;

	char in;
	cin >> in;

	if (in != '1' && in != '2')
		return 0;

	Topology* _t;

	if (in == '1')
		_t = new Client();
	else
		_t = new Server();

	_t->init();

	double dt = 0.0f;
	float timePass = 0.0f;
	int fps = 0;
	unsigned int start = clock();

	while (true)
	{
		_t->update(dt);

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