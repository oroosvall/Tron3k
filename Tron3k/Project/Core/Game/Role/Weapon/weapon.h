#ifndef WEAPON_H
#define WEAPON_H

#include "Bullet.h"
#include "../../GameDataIndex.h"

class Weapon
{
	protected:
		int maxClipSize;
		int currentClipAmmo;
		
		int currentBulletId = -1;
		
		float firingSpeed;
		float firingSpeedCurrentDelay;

		float reloadTime;
		float rldTimer;

		WEAPON_TYPE weaponType;

		void countDownFiringSpeed(float deltaTime);
		void countDownReloadTimer(float deltaTime);
	public:
		virtual void init() = 0;
		virtual bool shoot();
		virtual bool reload();

		virtual int update(float deltaTime) = 0;

		WEAPON_TYPE getType() { return weaponType; };

		int getBulletId();
};
#endif