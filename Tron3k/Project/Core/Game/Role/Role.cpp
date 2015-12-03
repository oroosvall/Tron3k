#include "Role.h"

Role::Role()
{
	currentWpn = 0;
	health = 1;
	movementSpeed = 0;
	weapons[0] = nullptr;
	weapons[1] = nullptr;
}
Role::Role(string inLoadedRoles[NROFROLES][NROFREADPROPERTIES])
{
	for (int i = 0; i < NROFROLES; i++)
	{
		for (int y = 0; y < NROFREADPROPERTIES; y++)
		{
			loadedRoles[i][y] = inLoadedRoles[i][y];
		}
	}
	currentWpn = 0;
	health = 1;
}

Role::~Role()
{
	if (weapons[0] != nullptr)
		delete weapons[0];
	if (weapons[1] != nullptr)
		delete weapons[1];
	if (storageMain != nullptr)
		delete storageMain;
	if (storageSec != nullptr)
		delete storageSec;
	if (specialAbility != nullptr)
		delete specialAbility;
}

void Role::loadWeapons(int role, int wpn)
{
	if (weapons[wpn] != nullptr)
		delete weapons[wpn];

	PROPERTIES w;
	if (wpn == 0)
		w = MAINWEP;
	if (wpn == 1)
		w = SECWEP;
	switch ((WEAPON_TYPE)atoi(loadedRoles[role][w].c_str()))
	{
	case PULSE_RIFLE:
		weapons[wpn] = new PulseRifle();
		break;
	case ENERGY_BOOST:
		weapons[wpn] = new EnergyBoost();
		break;
	}

	weapons[wpn]->init();
}

void Role::loadSpecialAbility(int role)
{
	if (specialAbility != nullptr)
		delete specialAbility;

	specialAbility = new Lightwall();
	specialAbility->init();
}

float Role::getMovementSpeed()
{
	return movementSpeed;
}

void Role::chooseRole(int role)
{
	if (role < NROFROLES)
	{
		health = atoi(loadedRoles[role][HEALTH].c_str());
		maxHealth = health;

		loadWeapons(role, 0);
		loadWeapons(role, 1);

		loadSpecialAbility(role);

		//consumable.setConsumable(atoi(loadedRoles[role][CONSUMABLE].c_str()));
		movementSpeed = atof(loadedRoles[role][MOVEMENTSPEED].c_str());
	}
}

void Role::swapWeapon(int swapTo)
{
	if (currentWpn != swapTo)
		currentWpn = swapTo;
}

Weapon* Role::getCurrentWeapon()
{
	return weapons[currentWpn];
}

void Role::takeDamage(int dmg)
{
	health -= dmg;
	if (health <= 0)
		health = 0;
}

void Role::heal(int h)
{
	health += h;
	if (h > maxHealth)
		h = maxHealth;
}