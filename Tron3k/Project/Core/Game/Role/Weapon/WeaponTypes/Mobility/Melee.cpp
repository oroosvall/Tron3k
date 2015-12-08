#include"Melee.h"

Melee::Melee()
{

}

Melee::~Melee()
{

}

void Melee::init()
{
	weaponType = WEAPON_TYPE::MELEE;

	maxClipSize = INT_MAX;
	currentClipAmmo = maxClipSize;

	currentBulletId = 0;

	firingSpeed = 1.0f;
	firingSpeedCurrentDelay = firingSpeed;

	reloadTime = 0.0f;
	rldTimer = reloadTime;
}

int Melee::update(float deltaTime)
{
	countDownFiringSpeed(deltaTime);
	countDownReloadTimer(deltaTime);
	return 0;
}