#include "Lightwall.h"

#include "../../../Player.h"

Lightwall::Lightwall()
{

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
		return true;
	return false;
}