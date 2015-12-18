#include "SprintControlLock.h"
#include "../../../Player.h"

void SprintControlLock::init(Player* myTarget)
{
	type = MODIFIER_TYPE::SPRINTCONTROLLOCK;
	sprintSpeed = 1.3f;

	target = myTarget;
	lifeTime = -1;

	glm::vec2 dir = glm::vec2(target->getDir().x, target->getDir().z);
	dir = normalize(dir);

	vel = glm::vec3(dir.x, 0, dir.y)*sprintSpeed;
	target->setVelocity(vel);
}


int SprintControlLock::getData(float dt)
{
 	bool kill = false;
	maxDuration -= dt;

	if (target->getVelocity().x != vel.x)
		kill = true;
	if (maxDuration <= 0.0f)
		kill = true;

	if (kill)
	{
		target->getRole()->setSpecialMeter(target->getRole()->getSpecialMeter() - 25);
		return 1;
	}

	return 0;
}

int SprintControlLock::setData(float dt)
{
	target->setVelocity(vel);

	return 0;
}