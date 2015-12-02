#include "LightWallLockedControls.h"
#include "../../../Player.h"

void LightWallLockedControls::init(Player* myTarget)
{
	target = myTarget;
	lifeTime = -1;

	vel = target->getVelocity();
}


int LightWallLockedControls::getData(float dt)
{
	if (!target->getGrounded())
		return 1;
	if (target->getVelocity() != vel)
		return 1;
	if (target->getRole()->getSpecialMeter() <= 0)
		return 1;

	return 0;
}

int LightWallLockedControls::setData(float dt)
{
	target->setVelocity(vel);
	return 0;
}