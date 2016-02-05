#include"Melee.h"

Melee::Melee()
{

}

Melee::~Melee()
{

}

bool Melee::shoot()
{
	bool ableToShoot = false;

	if (firingSpeedCurrentDelay < FLT_EPSILON)
	{
		firingSpeedCurrentDelay = firingSpeed;
		ableToShoot = true;
	}
	

	return ableToShoot;
}

void Melee::init()
{
	weaponType = WEAPON_TYPE::MELEE;

	maxClipSize = 1;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 1.0f;
	firingSpeedCurrentDelay = 0;

	reloadTime = 0.0f;
	rldTimer = 0;
}

int Melee::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	return 0;
}