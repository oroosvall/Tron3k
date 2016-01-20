#include "MultiJump.h"
#include "../../../Player.h"

MultiJump::MultiJump()
{
}


MultiJump::~MultiJump()
{
}

bool MultiJump::allowedToActivate(Player* p)
{
	if (this->coolDown < 0.01 && !p->getGrounded())
	{
		coolDown = 2.0f;
		return true;
	}
	return false;
}

void MultiJump::init()
{
	this->coolDown = 0;
	this->activationCost = 15;
	specialType = SPECIAL_TYPE::MULTIJUMP;
	key = GLFW_KEY_SPACE;
}

int MultiJump::update(float deltaTime)
{
	coolDowntick(deltaTime);
	return 0;
}

void MultiJump::coolDowntick(float deltaTime)
{
	if (coolDown > FLT_EPSILON)
	{
		coolDown -= deltaTime;
	}
}