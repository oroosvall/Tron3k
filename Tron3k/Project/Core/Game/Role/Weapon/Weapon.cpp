#include "Weapon.h"

Weapon::Weapon()
{
	maxClipSize = 0;
	currentClipAmmo = maxClipSize;
	weaponType = WEAPON_TYPE::PULSE_RIFLE;
}

Weapon::~Weapon()
{}

void Weapon::init(int maxClipSize, WEAPON_TYPE weaponType, float firingSpeed)
{
	this->maxClipSize = maxClipSize;
	currentClipAmmo = maxClipSize;
	this->weaponType = weaponType;
	this->firingSpeed = firingSpeed;
	currentDelay = firingSpeed;

	currentBulletId;
}

bool Weapon::shoot()
{
	bool ableToShoot = false;

	if (currentDelay <= 0 && currentClipAmmo > 0)
	{
		currentClipAmmo--;
		currentDelay = firingSpeed;
		ableToShoot = true;
	}

	return ableToShoot;
}

bool Weapon::reload()
{
	currentClipAmmo = maxClipSize;
	
	return true;
}

void Weapon::update(float deltaTime)
{
	if (currentDelay > 0)
	{
		currentDelay -= deltaTime;
	}
}

int Weapon::getBulletId()
{
	int bid = currentBulletId;
	currentBulletId++ % 255;
	return bid;
}