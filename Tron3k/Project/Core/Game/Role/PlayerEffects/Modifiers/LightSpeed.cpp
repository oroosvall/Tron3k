#include "LightSpeed.h"
#include "../../../Player.h"

void LightSpeed::init(Player* myTarget)
{
	type = MODIFIER_TYPE::LIGHTSPEEDMODIFIER;
	target = myTarget;

	target->getRole()->setMovementSpeed(lightSpeed);
}

int LightSpeed::getData(float dt)
{
	timer -= dt;

	if (timer <= 0)
	{
		target->getRole()->setMovementSpeed((atof(target->getRole()->getLoadedRoleSpecific(TRAPPER, MOVEMENTSPEED).c_str())));
		return 1;
	}

	return 0;
}

int LightSpeed::setData(float dt)
{
	return 0;
}