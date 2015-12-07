#pragma once
#include "../Special.h"

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
	int getActivationCost();

};

