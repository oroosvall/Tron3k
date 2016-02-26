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
	vel *= glm::vec3(1.6f, 0.0f, 1.6f);
	vel.y = 5.0f; //Jump height at initial dash
	target->setVelocity(vel);
	target->setGrounded(false);
}


int TrueGrit::getData(float dt)
{
	if (!target->isAlive())
		return 1;
	if (target->getHP() < oldHealth)
	{
		damageTaken = oldHealth - target->getHP();
	}

	return 0;
}

int TrueGrit::setData(float dt)
{
	if (!target->isAlive())
		return 1;
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
			lifeTime = 1.5f;
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
		target->setHP(oldHealth - damageTaken + 1);
		timer = healDelay;
	}
	else
		timer -= dt;

	return 0;
}

void TrueGrit::refresh()
{
	lifeTime = 0.2; 
	dashDone = false; 
	
	vel = target->getDir();
	vel *= glm::vec3(1.6f, 0.0f, 1.6f);
	vel.y = 5.0f; //Jump height at initial dash
	target->setVelocity(vel);
	target->setGrounded(false);
}