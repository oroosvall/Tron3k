#include"SprintD.h"
#include "../../../Player.h"

void SprintD::init()
{
	timer = 0;
	activationCost = 25;
	specialType = SPECIAL_TYPE::SPRINTD;
}

int SprintD::update(float dt)
{
	timer -= dt;

	return 0;
}
bool SprintD::allowedToActivate(Player* p)
{
	int specialMeter = p->getRole()->getSpecialMeter();

	if (this->timer < 0.0f && specialMeter >= 25)
	{
		p->getRole()->setSpecialMeter(specialMeter);
		timer = 4.0;
		return true;
	}
	return false;
}