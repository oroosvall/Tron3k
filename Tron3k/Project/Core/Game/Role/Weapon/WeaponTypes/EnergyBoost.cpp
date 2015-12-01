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

	firingSpeed = 1.0f;
	firingSpeedCurrentDelay = 0.0f;

	reloadTime = 1.0f;
	rldTimer = 0.0f;
}

int EnergyBoost::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}