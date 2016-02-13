#include "LightSpeedSpecial.h"

#include "../../../Player.h"
#include "../../Role.h"

void LightSpeedSpecial::init()
{
	specialType = SPECIAL_TYPE::LIGHTSPEEDSPECIAL;
}

int LightSpeedSpecial::update(float deltaTime)
{
	return 0;
}

bool LightSpeedSpecial::allowedToActivate(Player* p)
{
	if (p->getSpecialMeter() > 99.9f)
	{
		p->getRole()->setSpecialMeter(20.0f);
		return true;
	}
	return false;
}