#ifndef ROLE_H
#define ROLE_H

#include "Weapon/WeaponTypes/AllWeapons.h"
#include "Special/Special.h"
#include "Special/SpecialTypes/AllSpecials.h"
#include "PlayerEffects/Modifiers/AllModifiers.h"
#include "Weapon/Weapon.h"
#include "../GameDataIndex.h"
#include "Consumable/Consumable.h"
#include<string>

using namespace std;

enum ROLES { TRAPPER, DESTROYER, MOBILITY, BRUTE, MANIPULATOR, NROFROLES };
enum PROPERTIES{HEALTH, MAINWEP, SECWEP, CARRIEDITEM, MOVEMENTSPEED, NROFREADPROPERTIES};

class Role
{
	private:
		int currentWpn;
		int health;
		int maxHealth;
		float movementSpeed;
		string roles[NROFROLES];
		
		string loadedRoles[NROFROLES][NROFREADPROPERTIES];

		void loadWeapons(int role, int wpn); //wpn = main or sec
		void loadRoleSpecifics(int role);

		Weapon* weapons[2];
		Weapon* storageMain;
		Weapon* storageSec;

		Consumable* consumable;

		Special* specialAbility;
		float specialMeter;
		bool gainSpecial;

		Special* mobility;

	public:
		Role();
		Role(string inLoadedRoles[NROFROLES][NROFREADPROPERTIES]);
		~Role();

		float getMovementSpeed();

		void chooseRole(int role);
		void swapWeapon(WEAPON_TYPE wt, int swapTo);
		void swapWeaponLocal(int swapTo);
		Weapon* getCurrentWeapon();
		int getWeaponPosition() { return currentWpn; };
		Consumable* getConsumable() { return consumable; };

		void update(float dt);

		void setHealth(int newHealth) { health = newHealth; }; //Used by clients
		void takeDamage(int dmg); //Used by server
		void heal(int h); //Used by server

		int getHealth() { return health; };
		void returnToLife();

		Special* getSpecialAbility() { return specialAbility; };
		float getSpecialMeter() { return specialMeter; };
		void setSpecialMeter(float s) { specialMeter = s; };

		void shutOffMeterGain() { gainSpecial = false; };
		void activateMeterGain() { gainSpecial = true; };

		Special* getMobilityAbility() { return mobility; };
};
#endif