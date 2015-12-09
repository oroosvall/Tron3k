#include "LightWallLockedControls.h"
#include "../../../Player.h"

void LightWallLockedControls::init(Player* myTarget)
{
	type = MODIFIER_TYPE::LIGHTWALLCONTROLLOCK;
	lightWallSpeed = 1.3f;

	target = myTarget;
	lifeTime = -1;

	glm::vec2 dir = glm::vec2(target->getDir().x, target->getDir().z);
	dir = normalize(dir);

	vel = glm::vec3(dir.x, 0, dir.y)*lightWallSpeed;
	target->setVelocity(vel);
	target->getRole()->shutOffMeterGain();
}


int LightWallLockedControls::getData(float dt)
{
	bool kill = false;
	if (!target->getGrounded())
		kill = true;
	if (target->getVelocity() != vel)
		kill = true;
	if (target->getRole()->getSpecialMeter() <= 0)
		kill = true;

	if (kill)
	{
		target->getRole()->activateMeterGain();
		target->getRole()->setSpecialMeter(0.0f);
		return 1;
	}

	return 0;
}

int LightWallLockedControls::setData(float dt)
{
	bool kill = false;
	float specialLower = target->getRole()->getSpecialMeter();
	specialLower -= 20.0f*dt;
	target->setVelocity(vel);
	target->getRole()->setSpecialMeter(specialLower);

	if (Input::getInput()->justPressed(GLFW_KEY_SPACE))
		kill = true;
	if (Input::getInput()->justPressed(GLFW_MOUSE_BUTTON_LEFT))
		kill = true;

	if (kill)
	{
		target->getRole()->activateMeterGain();
		target->getRole()->setSpecialMeter(0.0f);
		return 1;
	}
	return 0;
}