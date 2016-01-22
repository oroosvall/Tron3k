#include "TrueGrit.h"
#include "../../../Player.h"

void TrueGrit::init(Player* myTarget)
{
	type = MODIFIER_TYPE::TRUEGRITMODIFIER;

	target = myTarget;
	lifeTime = 0.2f;
	dashDone = false;
	damageTaken = 0;

	oldHealth = target->getHP();

	vel = target->getDir();
	vel *= glm::vec3(3.0f, 0.0f, 3.0f);
	vel.y = 5.0f; //Jump height at initial dash
	target->setVelocity(vel);
	target->setGrounded(false);
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

	if (!dashDone)
	{
		if (lifeTime > FLT_EPSILON)
		{
			vel.y = target->getVelocity().y;
			target->setVelocity(vel);
		}
		else
		{
			dashDone = true;
			lifeTime = 2.8f;
		}
	}
	else
	{
		if (lifeTime < FLT_EPSILON)
		{
			return 1;
		}
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