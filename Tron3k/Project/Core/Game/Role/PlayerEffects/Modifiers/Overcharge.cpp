#include "Overcharge.h"
#include "../../../Player.h"

void Overcharge::init(Player* myTarget)
{
	type = MODIFIER_TYPE::OVERCHARGEMODIFIER;
	timer = 5.0f;
	target = myTarget;
	startHealth = target->getHP();
}

int Overcharge::getData(float dt)
{
	timer -= dt;

	target->getRole()->getCurrentWeapon()->reset();
	target->getRole()->setHealth(startHealth);

	if (timer <= 0)
	{
		return 1;
	}
	return 0;
}

int Overcharge::setData(float dt)
{
	target->getRole()->getCurrentWeapon()->reset();
	target->getRole()->setHealth(startHealth);
	return 0;
}