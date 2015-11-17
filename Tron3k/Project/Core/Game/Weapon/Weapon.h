#ifndef WEAPON_H
#define WEAPON_H

#include "Bullet.h"

class Weapon
{
	private:
		int maxClipSize;
		int currentClipAmmo;
		
		float firingSpeed;
		float currentDelay;

		int weaponType;
	public:
		Weapon();
		~Weapon();

		void init(int maxClipSize, int weaponType, float firingSpeed);
		void shoot();
		bool reload();

		void update(float deltaTime);
};
#endif