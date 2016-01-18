#include "LightSpeed.h"
#include "../../../Player.h"

void LightSpeed::init(Player* myTarget)
{
	type = MODIFIER_TYPE::LIGHTSPEEDMODIFIER;
	target = myTarget;
}

int LightSpeed::getData(float dt)
{
	return 0;
}

int LightSpeed::setData(float dt)
{
	timer -= dt;

	vec3 vel = target->getVelocity();
	vel.x *= 2.0;
	vel.z *= 2.0;
	target->setVelocity(vel);

	if (timer <= 0)
	{
		target->getRole()->setMovementSpeed((atof(target->getRole()->getLoadedRoleSpecific(TRAPPER, MOVEMENTSPEED).c_str())));
		return 1;
	}

	return 0;
}