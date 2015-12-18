#ifndef SPRINTD_H
#define SPRINTD_H
#include "../Special.h"
#include <GLFW\glfw3.h>

class SprintD:
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