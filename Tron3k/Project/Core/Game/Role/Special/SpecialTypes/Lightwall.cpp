#include "Lightwall.h"

#include "../../../Player.h"

Lightwall::Lightwall()
{
	specialId = 0;
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
	return 0;
}

bool Lightwall::allowedToActivate(Player* p)
{
	if (p->getGrounded())
	{
		if (activated)
		{
			p->getRole()->setSpecialMeter(p->getRole()->getSpecialMeter() - 8.0f);
		}

		if (p->getRole()->getSpecialMeter() - 100.0f < FLT_EPSILON && p->getRole()->getSpecialMeter() - 100.0f > -FLT_EPSILON)
		{
			activated = true;
			p->getRole()->setSpecialMeter(92.0f);
		}

		if (activated)
		{
			specialId++;
			return true;
		}
	}
	return false;
}