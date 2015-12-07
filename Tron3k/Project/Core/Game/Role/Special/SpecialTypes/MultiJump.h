#pragma once
#include "../Special.h"
#include <GLFW\glfw3.h>

class MultiJump :
	public Special
{

private:
	float coolDown;

public:
	MultiJump();
	~MultiJump();

	void init();
	int update(float deltaTime);

	bool allowedToActivate(Player* p);
};

