#include "Lightwall.h"

#include "../../Role.h"
#include "../../../Player.h"

Lightwall::Lightwall(Role* r)
{
	specialId = SPECIAL_TYPE::LIGHTWALL;
	myRole = r;
}

Lightwall::~Lightwall()
{

}

void Lightwall::init()
{
	specialType = SPECIAL_TYPE::LIGHTWALL;
}

int Lightwall::update(float deltaTime)
{
	if (cooldown > 0.0f)
		cooldown -= deltaTime;
	if (myRole->getSpecialMeter() < FLT_EPSILON)
	{
		activated = false;
	}
	return 0;
}

bool Lightwall::allowedToActivate(Player* p)
{
	if (!p->getRole()->getIfBusy())
	{
		if (p->getGrounded())
		{
			if (activated && cooldown < FLT_EPSILON)
			{
				if (p->getRole()->getSpecialMeter() > 15.0f)
				{
					int currentSpecial = p->getRole()->getSpecialMeter();
					p->getRole()->setSpecialMeter(currentSpecial - 15.0f);
				}
			}
			else if (p->getRole()->getSpecialMeter() - 100.0f < FLT_EPSILON && p->getRole()->getSpecialMeter() - 100.0f > -FLT_EPSILON)
			{
				activated = true;
				p->getRole()->setSpecialMeter(92.0f);
			}

			if (activated && cooldown < FLT_EPSILON)
			{
				specialId++;
				cooldown = 1.0f;
				return true;
			}
		}
	}
	return false;
}