#ifndef DASH_H
#define DASH_H
#include "../Special.h"
#include <GLFW\glfw3.h>

class Dash :
	public Special
{

private:
	float timer;

public:
	void init();
	int update(float deltaTime);

	bool allowedToActivate(Player* p);
};
#endif