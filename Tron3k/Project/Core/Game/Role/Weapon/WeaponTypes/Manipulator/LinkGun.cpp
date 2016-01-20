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

	maxClipSize = 4;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 2.0f;
	firingSpeedCurrentDelay = 0;

	reloadTime = 3.0f;
	rldTimer = 0;
}

int LinkGun::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}