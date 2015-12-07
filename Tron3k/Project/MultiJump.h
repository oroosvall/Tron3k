#pragma once
#include "C:\Users\Joel Meeeeeelll\Documents\GitHub\Tron3k\Tron3k\Project\Core\Game\Role\Special\Special.h"
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

