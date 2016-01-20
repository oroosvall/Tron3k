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

	if (timer <= 0)
	{
		return 1;
	}

	glm::vec3 vel = target->getVelocity();

	if (target->getGrounded())
	{
		vel.x *= lightSpeed;
		vel.z *= lightSpeed;
		target->setVelocity(vel);
	}
	return 0;
}