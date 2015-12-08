#include "BatteryFields.h"

BatteryFields::BatteryFields()
{

}

BatteryFields::~BatteryFields()
{

}

void BatteryFields::init()
{
	weaponType = WEAPON_TYPE::BATTERYFIELD_SLOW;

	maxClipSize = 2;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 3.0f;
	firingSpeedCurrentDelay = firingSpeed;

	reloadTime = 3.5f;
	rldTimer = reloadTime;
}

int BatteryFields::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}