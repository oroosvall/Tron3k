#ifndef WEAPON_H
#define WEAPON_H

#include "../../GameDataIndex.h"

#include <glm/glm.hpp>

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

		void reset() { currentClipAmmo = maxClipSize; };

		int getBulletId();
		int getCurrentAmmo() { return currentClipAmmo; };
		void setCurrentAmmo(int ammo) { currentClipAmmo = ammo; };
		bool getIfFullAmmo() { return currentClipAmmo == maxClipSize; };
		
		bool getIfReloading() { return rldTimer > 0; };
		bool getIfShooting() { return firingSpeedCurrentDelay > 0; };

		int getAmmo() { return currentClipAmmo; };
		int getMaxAmmo() { return maxClipSize; };
		void setFiringTimer(float tmr) { firingSpeedCurrentDelay = tmr; };
};
#endif