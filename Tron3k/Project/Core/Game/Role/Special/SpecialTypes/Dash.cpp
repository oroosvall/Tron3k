#include"SprintD.h"
#include "../../../Player.h"

void Dash::init()
{
	timer = -1;
	activationCost = 40;
	specialType = SPECIAL_TYPE::DASH;
}

int Dash::update(float dt)
{
	timer -= dt;

	return 0;
}
bool Dash::allowedToActivate(Player* p)
{
	int specialMeter = p->getRole()->getSpecialMeter();

	if (this->timer <= 0.0f && specialMeter >= 25)
	{
		p->getRole()->setSpecialMeter(specialMeter);
		timer = 1.0;
		return true;
	}
	return false;
}