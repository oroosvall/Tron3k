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
	int newAmmo = target->getRole()->getSpecificWeapon(0)->getCurrentAmmo();
	Role* temp = target->getRole();

	int newWeaponType = temp->getWeaponNRequiped();
	if (weaponType != newWeaponType)
	{
		if ( newAmmo > 1 && !temp->getIfBusy())
		{
			temp->getSpecificWeapon(0)->setFiringTimer(2.6f);
			temp->getSpecificWeapon(1)->setFiringTimer(2.6f);
			newAmmo -= 2;
		}
	}
	if (ammoCount != newAmmo)
		ammoCount = newAmmo;

	temp->getSpecificWeapon(0)->setCurrentAmmo(ammoCount);
	temp->getSpecificWeapon(1)->setCurrentAmmo(ammoCount);
	return 0;
}