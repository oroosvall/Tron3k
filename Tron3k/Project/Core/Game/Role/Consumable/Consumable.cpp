#include "Consumable.h"

Consumable::Consumable()
{}

Consumable::~Consumable()
{}

void Consumable::init(CONSUMABLE_TYPE ct)
{
	type = ct;
}

bool Consumable::use()
{
	if (used)
		return false;
	else
	{
		used = true;
		return true;
	}
}