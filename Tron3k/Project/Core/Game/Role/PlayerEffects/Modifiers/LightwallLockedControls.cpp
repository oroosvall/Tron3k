#include "LightWallLockedControls.h"
#include "../../../Player.h"

void LightWallLockedControls::init(Player* myTarget)
{
	lightWallSpeed = 1.3f;

	target = myTarget;
	lifeTime = -1;

	glm::vec2 dir = glm::vec2(target->getDir().x, target->getDir().z);
	dir = normalize(dir);

	vel = glm::vec3(dir.x, 0, dir.y)*lightWallSpeed;
	target->setVelocity(vel);
}


int LightWallLockedControls::getData(float dt)
{
	if (!target->getGrounded())
		return 1;
	if (target->getVelocity() != vel)
		return 1;
	//if (target->getRole()->getSpecialMeter() <= 0)
	//	return 1;

	return 0;
}

int LightWallLockedControls::setData(float dt)
{
	target->setVelocity(vel);

	if (Input::getInput()->justPressed(GLFW_KEY_SPACE))
		return 1;
	return 0;
}