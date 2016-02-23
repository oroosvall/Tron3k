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
	if (target->getAnimState_t_c() == AnimationState::third_primary_jump_begin)
		kill = true;
	if (target->getVelocity().x != vel.x || target->getVelocity().z != vel.z)
		kill = true;
	if (target->isLocal())
	{
		if (target->getRole()->getSpecialMeter() <= 0)
			kill = true;
		if (!target->getGrounded())
			kill = true;
	}

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