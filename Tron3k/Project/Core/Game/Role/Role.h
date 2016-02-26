#ifndef ROLE_H
#define ROLE_H

#include<sfml/Audio.hpp>
#include "Weapon/WeaponTypes/AllWeapons.h"
#include "../../SoundPlayer/sound/SoundPlayer.h"
#include "Special/Special.h"
#include "Special/SpecialTypes/AllSpecials.h"
#include "PlayerEffects/Modifiers/AllModifiers.h"
#include "Weapon/Weapon.h"
#include "../GameDataIndex.h"
#include "Consumable/Consumable.h"
#include<string>

using namespace std;

enum ROLES { TRAPPER, DESTROYER, MOBILITY, BRUTE, MANIPULATOR, NROFROLES };
enum PROPERTIES { HEALTH, MAINWEP, SECWEP, JUMPHEIGHT, MOVEMENTSPEED, NROFREADPROPERTIES };

class Role
{
private:
	int role;
	int currentWpn;
	int health;
	int maxHealth;
	float movementSpeed;
	float boundingBoxModifier;
	float boundingBoxRadius;
	float jumpHeight;
	float swapWeaponTimer;
	string roles[NROFROLES];
	string loadedRoles[NROFROLES][NROFREADPROPERTIES];
	glm::vec3 size;

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

	glm::vec3 GetSize()
	{
		return this->size;
	}

	float getMovementSpeed();

	void chooseRole(int r);
	void swapWeapon(WEAPON_TYPE wt, int swapTo);
	void swapWeaponLocal(int swapTo);
	int getWeaponNRequiped() { return currentWpn; };
	Weapon* getCurrentWeapon();
	Weapon* getSpecificWeapon(int wpn) { return weapons[wpn]; };
	int getWeaponPosition() { return currentWpn; };
	Consumable* getConsumable() { return consumable; };

	void update(float dt);

	void setHealth(int newHealth) { health = newHealth; if (health > maxHealth) health = maxHealth; }; //Used by clients
	void takeDamage(int dmg); //Used by server
	void heal(int h); //Used by server

	int getAmmo() { return weapons[currentWpn]->getAmmo(); };
	int getMaxAmmo() { return weapons[currentWpn]->getMaxAmmo(); };

	int getMaxHealth() { return maxHealth; };
	int getHealth() { return health; };
	float getJumpHeight() { return jumpHeight; };
	void returnToLife();

	Special* getSpecialAbility() { return specialAbility; };
	float getSpecialMeter() { return specialMeter; };
	void setSpecialMeter(float s) { specialMeter = s; };

	void shutOffMeterGain() { gainSpecial = false; };
	void activateMeterGain() { gainSpecial = true; };
	bool isGainingSpecial() { return gainSpecial; };

	Special* getMobilityAbility() { return mobility; };

	void setMovementSpeed(float speed) { movementSpeed = speed; };
	string getLoadedRoleSpecific(int i, int y) { return loadedRoles[i][y]; };

	float getBoxModifier() { return boundingBoxModifier; };
	float getBoxRadius() { return boundingBoxRadius; };

	bool getIfBusy();
	void forceBusy() { swapWeaponTimer = 0.1f; };

	int getRole();

	string getRoleAsString();
};
#endif