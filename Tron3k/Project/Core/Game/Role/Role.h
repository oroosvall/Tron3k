#ifndef ROLE_H
#define ROLE_H

#include "Weapon/weapon.h"
#include "../GameDataIndex.h"
#include "ActiveEffects/consumable.h"
#include<string>

using namespace std;

enum ROLES { TRAPPER, DESTROYER, MOBILITY, BRUTE, MANIPULATOR, NROFROLES };
enum PROPERTIES{HEALTH, MAINWEP, SECWEP, CONSUMABLE, MOVEMENTSPEED, NROFREADPROPERTIES};

class Role
{
	private:
		int currentWpn;
		int health;
		int maxHealth;
		float movementSpeed;
		string roles[NROFROLES];
		
		string loadedRoles[NROFROLES][NROFREADPROPERTIES];

		Weapon weapons[2];
		Weapon pickup;
		Weapon super;

		Consumable consumable;

	public:
		Role();
		Role(string inLoadedRoles[NROFROLES][NROFREADPROPERTIES]);
		~Role();

		float getMovementSpeed();

		void chooseRole(int role);
		void swapWeapon(int swapTo);
		Weapon* getCurrentWeapon();

		void setHealth(int newHealth) { health = newHealth; }; //Used by clients
		void takeDamage(int dmg); //Used by server
		void heal(int h); //Used by server

		int getHealth() { return health; };
		void returnToLife() { health = maxHealth; };
};
#endif