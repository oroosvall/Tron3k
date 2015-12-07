#include "LinkGun.h"

LinkGun::LinkGun()
{

}

LinkGun::~LinkGun()
{

}

void LinkGun::init()
{
	weaponType = WEAPON_TYPE::PULSE_RIFLE;

	maxClipSize = 4;
	currentClipAmmo = 4;

	currentBulletId = 0;

	firingSpeed = 2.0f;
	firingSpeedCurrentDelay = 0.0f;

	reloadTime = 3.0f;
	rldTimer = 0.0f;
}

int LinkGun::update(float deltaTime)
{
	return 0;
}