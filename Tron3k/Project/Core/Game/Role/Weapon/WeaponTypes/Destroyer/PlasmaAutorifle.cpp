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

	maxClipSize = 60;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 0.1f;
	firingSpeedCurrentDelay = 0.0f;

	reloadTime = 2.5f;
	rldTimer = 0.0f;
}

int PlasmaAutorifle::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}