#include "HackingDartSpecial.h"
#include "../../Role.h"
#include "../../../Player.h"

HackingDartSpecial::HackingDartSpecial(Role* r)
{
	specialId = SPECIAL_TYPE::HACKINGDARTSPECIAL;
	myRole = r;
}

HackingDartSpecial::~HackingDartSpecial()
{}

void HackingDartSpecial::init()
{
	specialType = SPECIAL_TYPE::HACKINGDARTSPECIAL;
}

int HackingDartSpecial::update(float deltaTime)
{
	return 0;
}

bool HackingDartSpecial::allowedToActivate(Player* p)
{
	bool allowed = false;
	
	if (p->getRole()->getSpecialMeter() >= 100)
	{
		p->getRole()->setSpecialMeter(0.0f);
		allowed = true;
	}

	return allowed;
}