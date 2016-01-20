#include "Shotgun.h"

Shotgun::Shotgun()
{

}

Shotgun::~Shotgun()
{

}

void Shotgun::init()
{
	weaponType = WEAPON_TYPE::SHOTGUN;

	maxClipSize = 5;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 1.5f;
	firingSpeedCurrentDelay = 0;

	reloadTime = 2.5f;
	rldTimer = 0;
}

int Shotgun::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}