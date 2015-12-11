#include "HackingDartModifier.h"
#include "../../../Player.h"

void HackingDartModifier::init(Player* myTarget)
{
	type = MODIFIER_TYPE::HACKINGDARTMODIFIER;
	target = myTarget;
}

int HackingDartModifier::getData(float dt)	//if return 1 remove modifier
{
	int end = 0;
	timer -= dt;

	if (timer <= 0)
		end = 1;
	return end;
}

int HackingDartModifier::setData(float dt) //if return 1 remove modifier
{	
	return 0;
}