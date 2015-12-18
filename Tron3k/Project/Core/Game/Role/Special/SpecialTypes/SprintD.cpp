#include"SprintD.h"
#include "../../../Player.h"

void SprintD::init()
{
	timer = 0;
	activationCost = 25;
	specialType = SPECIAL_TYPE::SPRINTD;
	key = GLFW_KEY_LEFT_SHIFT;
}

int SprintD::update(float dt)
{
	timer -= dt;

	return 0;
}
bool SprintD::allowedToActivate(Player* p)
{
	if (this->timer < 0.0f)
	{
		timer = 1.0;
		return true;
	}
	return false;
}