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
		specialId++;
		return true;
	}
	return false;
}