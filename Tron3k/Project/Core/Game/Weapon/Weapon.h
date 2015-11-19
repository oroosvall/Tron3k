#ifndef WEAPON_H
#define WEAPON_H

#include "Bullet.h"
#include "../GameDataIndex.h"

class Weapon
{
	private:
		int maxClipSize;
		int currentClipAmmo;
		
		int currentBulletId = 0;
		
		float firingSpeed;
		float currentDelay;

		WEAPON_TYPE weaponType;
	public:
		Weapon();
		~Weapon();

		void init(int maxClipSize, WEAPON_TYPE weaponType, float firingSpeed);
		bool shoot();
		bool reload();

		void update(float deltaTime);

		WEAPON_TYPE getType() { return weaponType; };

		int getBulletId();
};
#endif