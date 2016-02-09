#include "EnergyBoost.h"

EnergyBoost::EnergyBoost()
{

}

EnergyBoost::~EnergyBoost()
{

}

void EnergyBoost::init()
{
	weaponType = WEAPON_TYPE::ENERGY_BOOST;

	maxClipSize = 12;
	currentClipAmmo = 12;

	firingSpeed = 2.6f;
	firingSpeedCurrentDelay = 0.0f;

	reloadTime = 2.4f;
	rldTimer = 0.0f;
}

int EnergyBoost::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}

bool EnergyBoost::shoot()
{
	bool ableToShoot = false;

	if (firingSpeedCurrentDelay < FLT_EPSILON && currentClipAmmo > 1)
	{
		currentClipAmmo -= 2;
		firingSpeedCurrentDelay = firingSpeed;
		ableToShoot = true;
	}


	return ableToShoot;
}