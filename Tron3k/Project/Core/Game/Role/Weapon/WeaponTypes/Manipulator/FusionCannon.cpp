#include "FusionCannon.h"

FusionCannon::FusionCannon()
{}

FusionCannon::~FusionCannon()
{}

bool FusionCannon::shoot()
{
	bool ableToShoot = false;

	if (firingSpeedCurrentDelay < FLT_EPSILON)
	{
		firingSpeedCurrentDelay = firingSpeed;
		ableToShoot = true;
	}


	return ableToShoot;
}

void FusionCannon::init()
{
	weaponType = WEAPON_TYPE::FUSION_CANNON;

	maxClipSize = 1;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 2.0f;
	firingSpeedCurrentDelay = 0.0f;

	reloadTime = 0.0f;
	rldTimer = 0;
}

int FusionCannon::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}