#include "LightSpeed.h"
#include "../../../Player.h"

void LightSpeed::init(Player* myTarget)
{
	type = MODIFIER_TYPE::LIGHTSPEEDMODIFIER;
	target = myTarget;
	speed = vec3(1.5, 0.0, 1.5);
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

	vel += speed * vel * dt;
	vel.y = target->getVelocity().y;
	target->setVelocity(vel);

	return 0;
}