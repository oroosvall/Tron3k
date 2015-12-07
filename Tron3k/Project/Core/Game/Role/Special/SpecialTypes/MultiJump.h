#pragma once
#include "../Special.h"
#include <GLFW\glfw3.h>

class MultiJump :
	public Special
{

private:
	int activationCost;
	float coolDown;

public:
	MultiJump();
	~MultiJump();

	void init();
	int update(float deltaTime);

	bool allowedToActivate();
	int getActivationCost() { return activationCost; }
};

