#include "LightWallLockedControls.h"
#include "../../../Player.h"

void LightWallLockedControls::init(Player* myTarget)
{
	type = MODIFIER_TYPE::LIGHTWALLCONTROLLOCK;
	lightWallSpeed = 2.5f;

	target = myTarget;
	lifeTime = -1;

	glm::vec2 dir = glm::vec2(target->getDir().x, target->getDir().z);
	dir = normalize(dir);

	vel = glm::vec3(dir.x, 0.0f, dir.y)*lightWallSpeed;
	normvel = normalize(vel);
	target->setVelocity(vel);
	target->getRole()->shutOffMeterGain();
}

LightWallLockedControls::~LightWallLockedControls()
{
	target->getRole()->activateMeterGain();
}

int LightWallLockedControls::getData(float dt)
{
	bool kill = false;
	glm::vec3 newVel = target->getVelocity();
	if (length(newVel) < FLT_EPSILON)
		kill = true;
	else
	{
		newVel = normalize(newVel);
		if (newVel.x != normvel.x || newVel.z != normvel.z)
			kill = true;
	}
	//if (target->getVelocity().x != vel.x || target->getVelocity().z != vel.z)
	//	kill = true;
	if (target->getRole()->getSpecialMeter() <= 0)
		kill = true;
	if (!target->getGrounded())
		kill = true;

	if (kill)
	{
		target->getRole()->activateMeterGain();
		return 1;
	}

	return 0;
}

int LightWallLockedControls::setData(float dt)
{
	bool kill = false;
	float specialLower = target->getRole()->getSpecialMeter();
	specialLower -= 40.0f*dt;
	target->setVelocity(vel);
	target->getRole()->setSpecialMeter(specialLower);

	if (Input::getInput()->justPressed(GLFW_KEY_SPACE))
		kill = true;
	if (Input::getInput()->justPressed(GLFW_MOUSE_BUTTON_LEFT))
		kill = true;
	if (Input::getInput()->justPressed(GLFW_KEY_E))
		kill = true;

	if (kill)
	{
		target->getRole()->activateMeterGain();
		return 1;
	}
	return 0;
}