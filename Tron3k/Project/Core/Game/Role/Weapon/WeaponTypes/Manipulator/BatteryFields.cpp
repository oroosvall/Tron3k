#include "BatteryFields.h"

BatteryFields::BatteryFields()
{

}

BatteryFields::~BatteryFields()
{

}

bool BatteryFields::shoot()
{
	bool ableToShoot = false;

	if (rldTimer < FLT_EPSILON)
	{
		if (shootingDelay < FLT_EPSILON && currentClipAmmo > 0)
		{
			firingSpeedCurrentDelay = firingSpeed;
			shootingDelay = 1.0f;
			ableToShoot = true;
			if (weaponType == WEAPON_TYPE::BATTERYWPN_SLOW)
				weaponType = WEAPON_TYPE::BATTERYWPN_SPEED;
			else if (weaponType == WEAPON_TYPE::BATTERYWPN_SPEED)
				weaponType = WEAPON_TYPE::BATTERYWPN_SLOW;
		}
	}

	return ableToShoot;
}

bool BatteryFields::reload()
{
	rldTimer = reloadTime;
	weaponType = WEAPON_TYPE::BATTERYWPN_SPEED;
	return true;
}

void BatteryFields::init()
{
	weaponType = WEAPON_TYPE::BATTERYWPN_SPEED;

	maxClipSize = 1;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 0.5f;
	firingSpeedCurrentDelay = 0;
}

int BatteryFields::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	shootingDelay -= deltaTime;
	return 0;
}