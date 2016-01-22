#include "LinkGun.h"

LinkGun::LinkGun()
{

}

LinkGun::~LinkGun()
{

}

void LinkGun::init()
{
	weaponType = WEAPON_TYPE::LINK_GUN;

	maxClipSize = 1;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 0.0f;
	firingSpeedCurrentDelay = 0.0f;

	reloadTime = 3.0f;
	rldTimer = 0;
}

int LinkGun::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}