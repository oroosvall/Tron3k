#include "GrapplingHookSpecial.h"
#include "../../Role.h"
#include "../../../Player.h"

GrapplingHookSpecial::GrapplingHookSpecial(){}

GrapplingHookSpecial::~GrapplingHookSpecial(){}

void GrapplingHookSpecial::init()
{
	specialType = SPECIAL_TYPE::GRAPPLINGHOOKSPECIAL;
	activationCost = 15.0f;
	key = GLFW_KEY_LEFT_SHIFT;
}

int GrapplingHookSpecial::update(float deltaTime)
{
	if (cooldown > FLT_EPSILON)
	{
		cooldown -= deltaTime;
	}
	return 0;
}

bool GrapplingHookSpecial::allowedToActivate(Player* p)
{
	bool allowed = false;

	if (cooldown < FLT_EPSILON)
	{
		p->movmentSpecialAnimUse();
		cooldown = 2.0f;
		allowed = true;
	}

	return allowed;
}