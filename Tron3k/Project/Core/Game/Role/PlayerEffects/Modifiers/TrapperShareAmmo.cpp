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
		newAmmo -= 2;
		Role* temp = target->getRole();
		temp->getSpecificWeapon(0)->setFiringTimer(2.6f);
	}
	if(ammoCount != newAmmo)
		ammoCount = newAmmo;
	target->getRole()->getSpecificWeapon(0)->setCurrentAmmo(ammoCount);
	return 0;
}