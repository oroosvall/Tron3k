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
	if (myPlayer == nullptr)
		myPlayer = p;
	if (!myPlayer->getGrounded() && !used)
	{
		used = true;
		myPlayer->movmentSpecialAnimUse();
		return true;
	}
	return false;
}

void MultiJump::init()
{
	activationCost = 15;
	specialType = SPECIAL_TYPE::MULTIJUMP;
	isOnJumpKey = true;
}

int MultiJump::update(float deltaTime)
{
	if (myPlayer != nullptr)
	{
		if (myPlayer->getGrounded())
			used = false;
	}
	return 0;
}