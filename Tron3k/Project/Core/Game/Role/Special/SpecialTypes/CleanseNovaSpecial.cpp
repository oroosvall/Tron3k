#include "CleanseNovaSpecial.h"

#include "../../Role.h"
#include "../../../Player.h"

CleanseNovaSpecial::CleanseNovaSpecial(Role* r)
{
	specialId = SPECIAL_TYPE::CLEANSESPECIAL;
	myRole = r;
}

CleanseNovaSpecial::~CleanseNovaSpecial()
{

}

void CleanseNovaSpecial::init()
{
	specialType = SPECIAL_TYPE::CLEANSESPECIAL;
}

int CleanseNovaSpecial::update(float deltaTime)
{
	return 0;
}

bool CleanseNovaSpecial::allowedToActivate(Player* p)
{
	if (myRole->getSpecialMeter() - 100.0f < FLT_EPSILON && myRole->getSpecialMeter() - 100.0f > -FLT_EPSILON)
	{
		myRole->setSpecialMeter(0.0f);
		return true;
	}
	return false;
}