#include "Role.h"

Role::Role()
{
	currentWpn = 0;
	health = 1;
	movementSpeed = 0;
	weapons[0].init(36, WEAPON_TYPE::PULSE_RIFLE, 0.5);
}
Role::Role(string inLoadedRoles[NROFROLES][NROFREAD])
{
	for (int i = 0; i < NROFROLES; i++)
	{
		for (int y = 0; y < NROFREAD; y++)
		{
			loadedRoles[i][y] = inLoadedRoles[i][y];
		}
	}
	currentWpn = 0;
	health = 1;
	chooseRole(TRAPPER);
}

Role::~Role()
{

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

		weapons[0].init(36, (WEAPON_TYPE)atoi(loadedRoles[role][MAINWEP].c_str()), 0.5);
		weapons[1].init(80, (WEAPON_TYPE)atoi(loadedRoles[role][SECWEP].c_str()), 0.2);

		consumable.setConsumable(atoi(loadedRoles[role][CONSUMABLE].c_str()));
		movementSpeed = atof(loadedRoles[role][MOVEMENTSPEED].c_str());
	}
}

void Role::swapWeapon(int swapTo)
{
	if (currentWpn != swapTo)
	{
		switch (swapTo)
		{
		case WEAPON_TYPE::PULSE_RIFLE:
			currentWpn = 0;
			break;
		case WEAPON_TYPE::POOP_GUN:
			currentWpn = 1;
			break;
		}
	}
}

Weapon* Role::getCurrentWeapon()
{
	return &weapons[currentWpn];
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