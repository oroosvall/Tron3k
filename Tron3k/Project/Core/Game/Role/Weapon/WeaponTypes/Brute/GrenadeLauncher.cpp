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

	maxClipSize = 8;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 1.0f;
	firingSpeedCurrentDelay = firingSpeed;

	reloadTime = 3.5f;
	rldTimer = reloadTime;
}

int GrenadeLauncher::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}