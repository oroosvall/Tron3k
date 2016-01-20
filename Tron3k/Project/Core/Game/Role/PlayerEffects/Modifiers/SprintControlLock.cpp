#include "SprintControlLock.h"
#include "../../../Player.h"

void SprintControlLock::init(Player* myTarget)
{
	type = MODIFIER_TYPE::SPRINTCONTROLLOCK;
	sprintSpeed = 2.5f;

	target = myTarget;
	lifeTime = 4.0f;

	dir = target->getDir();

	vel = glm::vec3(dir.x, 0.0f, dir.z)*sprintSpeed;
	target->setVelocity(vel);
}


int SprintControlLock::getData(float dt)
{
 	bool kill = false;
	lifeTime -= dt;

	if (target->getVelocity().x != vel.x || target->getVelocity().z != vel.z)
		kill = true;
	if (lifeTime <= 0.0f)
		kill = true;
	if (!target->getGrounded())
		kill = true;

	if (kill)
		return 1;

	return 0;
}

int SprintControlLock::setData(float dt)
{
	vel += vec3(0.3, 0.0, 0.3) * vel * dt;
	vel.y = target->getVelocity().y;
	target->setVelocity(vel);
	target->setDir(dir);

	return 0;
}