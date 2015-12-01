#include "Weapon.h"

bool Weapon::shoot()
{
	bool ableToShoot = false;

	if (rldTimer < FLT_EPSILON)
	{
		if (firingSpeedCurrentDelay <= 0 && currentClipAmmo > 0)
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
	if (currentBulletId == -1)
		currentBulletId++ % 255;
	return bid;
}

void Weapon::countDownFiringSpeed(float dt)
{
	if (firingSpeedCurrentDelay > FLT_EPSILON)
	{
		firingSpeedCurrentDelay -= dt;
	}
}
void Weapon::countDownReloadTimer(float dt)
{
	if (rldTimer > FLT_EPSILON) //Tick dat reloadtimer down boi
	{
		rldTimer -= dt;
		if (rldTimer < FLT_EPSILON)
			currentClipAmmo = maxClipSize; //Reload complete
	}
}