#include "TrapperShareAmmo.h"
#include "../../../Player.h"

void TrapperShareAmmo::init(Player* myTarget)
{
	type = MODIFIER_TYPE::TRAPPERSHAREAMMO;

	target = myTarget;
	lifeTime = -1;
	ammoCount = target->getRole()->getCurrentWeapon()->getCurrentAmmo();
	weaponType = target->getRole()->getCurrentWeapon()->getType();
}


int TrapperShareAmmo::getData(float dt)
{
	return 0;
}

int TrapperShareAmmo::setData(float dt)
{
	int newAmmo = target->getRole()->getCurrentWeapon()->getCurrentAmmo();

	if (ammoCount != newAmmo)
	{
		int newWeaponType = target->getRole()->getCurrentWeapon()->getType();
		if (weaponType != newWeaponType)
		{
			weaponType = newWeaponType;
			target->getRole()->getCurrentWeapon()->setCurrentAmmo(ammoCount);
		}
		else
			ammoCount = newAmmo;
	}
	return 0;
}