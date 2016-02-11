#include "Weapon.h"

bool Weapon::shoot()
{
	bool ableToShoot = false;

	if (rldTimer < FLT_EPSILON)
	{
		if (firingSpeedCurrentDelay < FLT_EPSILON && currentClipAmmo > 0)
		{
			currentClipAmmo--;
			firingSpeedCurrentDelay = firingSpeed;
			ableToShoot = true;
		}
	}
	
	return ableToShoot;
}

bool Weapon::reload()
{
	rldTimer = reloadTime;

	return true;
}

int Weapon::getBulletId()
{
	int bid = currentBulletId;
	if (currentBulletId != -1)
	{
		currentBulletId++;
		currentBulletId % 255;
	}
	return bid;
}

void Weapon::countDownFiringSpeed(float dt)
{
	if (firingSpeedCurrentDelay >= 0)
	{
		firingSpeedCurrentDelay -= dt;
	}
}
void Weapon::countDownReloadTimer(float dt)
{
	if (rldTimer >= 0) //Tick dat reloadtimer down boi
	{
		rldTimer -= dt;
		if (rldTimer <= 0) //Reload complete
		{
			currentClipAmmo = maxClipSize;
		}
	}
}