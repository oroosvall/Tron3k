#include "TrapperShareAmmo.h"
#include "../../../Player.h"

void TrapperShareAmmo::init(Player* myTarget)
{
	type = MODIFIER_TYPE::TRAPPERSHAREAMMO;

	target = myTarget;
	lifeTime = -1;
	sticky = true;
	ammoCount = target->getRole()->getCurrentWeapon()->getCurrentAmmo();
	weaponType = target->getRole()->getWeaponNRequiped();
}


int TrapperShareAmmo::getData(float dt)
{
	return 0;
}

int TrapperShareAmmo::setData(float dt)
{
	int newAmmo = target->getRole()->getCurrentWeapon()->getCurrentAmmo();

	int newWeaponType = target->getRole()->getWeaponNRequiped();
	if (weaponType != newWeaponType)
	{
		weaponType = newWeaponType;
		target->getRole()->getCurrentWeapon()->setCurrentAmmo(ammoCount);
	}
	if(ammoCount != newAmmo)
		ammoCount = newAmmo;

	return 0;
}