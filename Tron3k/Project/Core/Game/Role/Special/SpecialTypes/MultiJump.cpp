#include "MultiJump.h"



MultiJump::MultiJump()
{
}


MultiJump::~MultiJump()
{
}

bool MultiJump::allowedToActivate()
{
	if (this->coolDown < 0.01)
	{
		return true;
	}
	return false;
}

void MultiJump::init()
{
	this->coolDown = 0;
	this->activationCost = 15;
	specialType = SPECIAL_TYPE::MULTIJUMP;
}

int MultiJump::update(float deltaTime)
{
	return 0;
}