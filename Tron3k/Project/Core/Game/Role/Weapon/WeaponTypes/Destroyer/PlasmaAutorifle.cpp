#include "PlasmaAutorifle.h"

PlasmaAutorifle::PlasmaAutorifle()
{

}

PlasmaAutorifle::~PlasmaAutorifle()
{

}

void PlasmaAutorifle::init()
{
	weaponType = WEAPON_TYPE::PLASMA_AUTORIFLE;

	maxClipSize = 36;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 0.25f;
	firingSpeedCurrentDelay = firingSpeed;

	reloadTime = 1.0f;
	rldTimer = reloadTime;
}

int PlasmaAutorifle::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}