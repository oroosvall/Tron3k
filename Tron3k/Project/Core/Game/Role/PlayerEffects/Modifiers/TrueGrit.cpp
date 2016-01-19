#include "TrueGrit.h"
#include "../../../Player.h"

void TrueGrit::init(Player* myTarget)
{
	type = MODIFIER_TYPE::TRUEGRITMODIFIER;

	target = myTarget;
	lifeTime = 3.0f;
	endTime = lifeTime - 0.4;
	damageTaken = 0;

	oldHealth = target->getHP();
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
	lifeTime -= dt;

	if (lifeTime <= 0)
	{
		return 1;
	}

	if (dashDone != true)
	{
		vec3 vel = target->getVelocity();
		if (lifeTime > endTime)
		{
			vel += vec3(1.5, 1.5, 1.5) * vel * dt;
		}
		else
			dashDone = true;
	}

	if (damageTaken > 0 && timer <= 0)		//Heal atm since damage reduction was impossibru
	{
		oldHealth++;
		target->setHP(oldHealth);
		timer = healDelay;
	}
	else
		timer--;

	return 0;
}