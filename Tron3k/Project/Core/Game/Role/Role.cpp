#include "Role.h"
#include "Special\SpecialTypes\MultiJump.h"

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
	if (mobility != nullptr)
		delete mobility;
	if (consumable != nullptr)
		delete consumable;
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

void Role::loadRoleSpecifics(int role)
{
	if (specialAbility != nullptr)
		delete specialAbility;
	if (mobility != nullptr)
		delete mobility;
	if (consumable != nullptr)
		delete consumable;

	switch (role)
	{
	case TRAPPER:
		specialAbility = new Lightwall();
		specialAbility->init();
		mobility = new MultiJump();
		mobility->init();
		break;
	}
}

float Role::getMovementSpeed()
{
	return movementSpeed;
}

void Role::update(float dt)
{
	weapons[0]->update(dt);
	weapons[1]->update(dt);
	specialAbility->update(dt);
	mobility->update(dt);

	if (gainSpecial && specialMeter < 100.0f)
	{
		specialMeter += dt*10.0f;
		if (specialMeter >= 100.0f)
			specialMeter = 100.0f;
	}
}

void Role::chooseRole(int role)
{
	if (role < NROFROLES)
	{
		health = atoi(loadedRoles[role][HEALTH].c_str());
		maxHealth = health;

		loadWeapons(role, 0);
		loadWeapons(role, 1);

		loadRoleSpecifics(role);

		consumable = new Consumable();
		consumable->init(CONSUMABLE_TYPE(atoi(loadedRoles[role][CARRIEDITEM].c_str())));

		//consumable.setConsumable(atoi(loadedRoles[role][CONSUMABLE].c_str()));
		movementSpeed = float(atof(loadedRoles[role][MOVEMENTSPEED].c_str()));

		gainSpecial = true;
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

void Role::returnToLife()
{
	specialMeter = 0.0f;
	gainSpecial = true;
	health = maxHealth;
	consumable->reset();
	weapons[0]->reset();
	weapons[1]->reset();
}