#include "EnergyShield.h"

EnergyShield::EnergyShield()
{

}

EnergyShield::~EnergyShield()
{

}

void EnergyShield::init()
{
	weaponType = WEAPON_TYPE::ENERGY_SHIELD;

	maxClipSize = 10;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 1.0f;
	firingSpeedCurrentDelay = firingSpeed;

	reloadTime = 3.5f;
	rldTimer = reloadTime;
}

int EnergyShield::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}