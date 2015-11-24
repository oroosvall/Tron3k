#ifndef ROLE_H
#define ROLE_H

#include "Weapon/weapon.h"
#include "../GameDataIndex.h"
#include "ActiveEffects/consumable.h"
#include<string>
#include <fstream>
#include <sstream>

using namespace std;

enum ROLES { TRAPPER, DESTROYER, MOBILITY, BRUTE, MANIPULATOR, NROFROLES };
enum PROPERTIES{HEALTH, MAINWEP, SECWEP, CONSUMABLE, MOVEMENTSPEED, NROFREAD};

class Role
{
	private:
		int currentWpn;
		int health;
		float movementSpeed;
		string roles[NROFROLES];
		
		string loadedRoles[NROFROLES][NROFREAD];

		Weapon weapons[2];
		Weapon pickup;
		Weapon super;

		Consumable consumable;

	public:
		Role();
		~Role();

		float getMovementSpeed();

		void chooseRole(int role);
		void swapWeapon(int swapTo);
		Weapon* getCurrentWeapon();
};
#endif