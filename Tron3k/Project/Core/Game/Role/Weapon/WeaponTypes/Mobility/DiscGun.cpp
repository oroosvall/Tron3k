#include "DiscGun.h"

DiscGun::DiscGun()
{

}

DiscGun::~DiscGun()
{

}

void DiscGun::init()
{
	weaponType = WEAPON_TYPE::DISC_GUN;

	maxClipSize = 10;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 1.2f;
	firingSpeedCurrentDelay = 0;

	reloadTime = 2.0f;
	rldTimer = 0;
}

int DiscGun::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}