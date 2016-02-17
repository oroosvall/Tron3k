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
	if (myRole->isGainingSpecial())
	{
		activated = false;
	}
	return 0;
}

bool Lightwall::allowedToActivate(Player* p)
{
	if (!p->getRole()->getIfBusy() && !activated)
	{
		if (!p->searchModifier(MODIFIER_TYPE::LIGHTWALLCONTROLLOCK))
		{
			if (p->getGrounded())
			{
				float special = p->getRole()->getSpecialMeter();
				if (special > 25.0f)
				{
					activated = true;
				}

				if (activated)
				{
					specialId++;
					return true;
				}
			}
		}
	}
	return false;
}