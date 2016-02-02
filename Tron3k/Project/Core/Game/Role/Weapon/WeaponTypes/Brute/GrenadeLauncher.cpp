#include "GrenadeLauncher.h"

GrenadeLauncher::GrenadeLauncher()
{

}

GrenadeLauncher::~GrenadeLauncher()
{

}

void GrenadeLauncher::init()
{
	weaponType = WEAPON_TYPE::GRENADE_LAUNCHER;

	maxClipSize = 6;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 1.2f;
	firingSpeedCurrentDelay = 0;

	reloadTime = 4.0f;
	rldTimer = 0;
}

int GrenadeLauncher::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}