#include "SprintControlLock.h"
#include "../../../Player.h"

void SprintControlLock::init(Player* myTarget)
{
	type = MODIFIER_TYPE::SPRINTCONTROLLOCK;
	sprintSpeed = 1.3f;

	target = myTarget;
	lifeTime = 5;

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

	return 0;
}

int SprintControlLock::setData(float dt)
{
	vel *= 1.1 * dt;
	target->setVelocity(vel);

	return 0;
}