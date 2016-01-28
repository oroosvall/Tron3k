#include "ThunderDome.h"

#include "../../Role.h"
#include "../../../Player.h"

ThunderDome::ThunderDome(Role* r)
{
	specialId = SPECIAL_TYPE::THUNDERDOME;
	myRole = r;
}

ThunderDome::~ThunderDome()
{

}

void ThunderDome::init()
{
	specialType = SPECIAL_TYPE::THUNDERDOME;
}

int ThunderDome::update(float deltaTime)
{
	if (myRole->getSpecialMeter() < FLT_EPSILON)
	{
		activated = false;
	}
	return 0;
}

bool ThunderDome::allowedToActivate(Player* p)
{
	if (!p->getRole()->getIfBusy())
	{
		if (p->getGrounded())
		{
			if (activated)
			{
				if (p->getRole()->getSpecialMeter() > 99.0f)
				{
					p->getRole()->setSpecialMeter(0.0f);
				}
			}
			else if (p->getRole()->getSpecialMeter() - 100.0f < FLT_EPSILON && p->getRole()->getSpecialMeter() - 100.0f > -FLT_EPSILON)
			{
				activated = true;
				p->getRole()->setSpecialMeter(0.0f);
			}

			if (activated)
			{
				specialId++;
				return true;
			}
		}
	}
	return false;
}