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

	firingSpeed = 2.0f;
	firingSpeedCurrentDelay = 0;

	reloadTime = 3.0f;
	rldTimer = 0;
}

int EnergyShield::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}