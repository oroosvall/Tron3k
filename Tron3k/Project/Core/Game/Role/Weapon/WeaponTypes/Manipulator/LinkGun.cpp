#include "LinkGun.h"

LinkGun::LinkGun()
{}

LinkGun::~LinkGun()
{}

bool LinkGun::shoot()
{
	bool ableToShoot = false;

	if (firingSpeedCurrentDelay < FLT_EPSILON)
	{
		firingSpeedCurrentDelay = firingSpeed;
		ableToShoot = true;
	}


	return ableToShoot;
}

void LinkGun::init()
{
	weaponType = WEAPON_TYPE::LINK_GUN;

	maxClipSize = 1;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 2.0f;
	firingSpeedCurrentDelay = 0.0f;

	reloadTime = 0.0f;
	rldTimer = 0;
}

int LinkGun::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}