#include "TrueGrit.h"
#include "../../../Player.h"

void TrueGrit::init(Player* myTarget)
{
	type = MODIFIER_TYPE::TRUEGRITMODIFIER;

	target = myTarget;
	lifeTime = 3.0f;
	damageTaken = 0;

	oldHealth = target->getHP();

	vec2 dir = vec2(target->getDir().x, target->getDir().z);
	dir = normalize(dir);

}


int TrueGrit::getData(float dt)
{
	if (target->getHP() < oldHealth)
	{
		damageTaken = oldHealth - target->getHP();
	}

	return 0;
}

int TrueGrit::setData(float dt)
{
	if (damageTaken > 0)
	{
		damageTaken *= 0.5;
		oldHealth -= damageTaken;
		target->setHP(oldHealth);
	}

	return 0;
}