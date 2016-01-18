#include "Role.h"
#include "Special\SpecialTypes\MultiJump.h"
#include "Special/SpecialTypes/WallJump.h"

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
	else if (wpn == 1)
		w = SECWEP;
	switch ((WEAPON_TYPE)atoi(loadedRoles[role][w].c_str()))
	{
	case PULSE_RIFLE:
		weapons[wpn] = new PulseRifle();
		break;
	case ENERGY_BOOST:
		weapons[wpn] = new EnergyBoost();
		break;
	case GRENADE_LAUNCHER:
		weapons[wpn] = new GrenadeLauncher();
		break;
	case SHOTGUN:
		weapons[wpn] = new Shotgun();
		break;
	case ENERGY_SHIELD:
		weapons[wpn] = new EnergyShield();
		break;
	case PLASMA_AUTORIFLE:
		weapons[wpn] = new PlasmaAutorifle();
		break;
	case BATTERYFIELD_SLOW:
		weapons[wpn] = new BatteryFields();
		break;
	case LINK_GUN:
		weapons[wpn] = new LinkGun();
		break;
	case DISC_GUN:
		weapons[wpn] = new DiscGun();
		break;
	case MELEE:
		weapons[wpn] = new Melee();
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
		case TRAPPER:
	{		role = 0;
			specialAbility = new Lightwall(this);
			specialAbility->init();
			mobility = new MultiJump();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::CLUSTERGRENADE);
			break;
		case DESTROYER:
			role = 1;
			specialAbility = new Lightwall(this);
			specialAbility->init();
			mobility = new SprintD();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::OVERCHARGE);
			break;
		case MOBILITY:
			role = 2;
			specialAbility = new HackingDartSpecial(this);
			specialAbility->init();
			mobility = new WallJump();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::LIGHTSPEED);
			break;
		case BRUTE:
			role = 3;
			specialAbility = new Lightwall(this);
			specialAbility->init();
			mobility = new MultiJump();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::THUNDERDOME);
			break;
		case MANIPULATOR:
			role = 4;
			specialAbility = new Lightwall(this);
			specialAbility->init();
			mobility = new MultiJump();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::VACUUMGRENADE);
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
		this->role = role;
		health = atoi(loadedRoles[role][HEALTH].c_str());
		maxHealth = health;

		loadWeapons(role, 0);
		loadWeapons(role, 1);

		loadRoleSpecifics(role);

		movementSpeed = float(atof(loadedRoles[role][MOVEMENTSPEED].c_str()));
		jumpHeight = float(atof(loadedRoles[role][JUMPHEIGHT].c_str()));

		boundingBoxModifier = 0.55f;

		gainSpecial = true;
	}
}

void Role::swapWeapon(WEAPON_TYPE wt, int swapTo)
{
	if (swapTo == 2)
	{
		//Swap secwpn with storage
		//Load tech pickup in secwpn slot
	}
	if (swapTo == 3)
	{
		//Swap mainwpn with storage
		//Load super in mainwpn slot
	}
	else
	{
		if (currentWpn != swapTo)
			currentWpn = swapTo;
	}
}

void Role::swapWeaponLocal(int swapTo)
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

int Role::getRole()
{
	return this->role;
}