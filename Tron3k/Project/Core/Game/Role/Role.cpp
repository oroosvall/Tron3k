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
	swapWeaponTimer = 0.0f;
	role = ROLES::NROFROLES;
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
	case BATTERYWPN_SLOW:
		weapons[wpn] = new BatteryFields();
		break;
	case FUSION_CANNON:
		weapons[wpn] = new FusionCannon();
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

void Role::loadRoleSpecifics(int _role)
{
	if (specialAbility != nullptr)
		delete specialAbility;
	if (mobility != nullptr)
		delete mobility;
	if (consumable != nullptr)
		delete consumable;

	switch (_role)
		case TRAPPER:
	{		role = 0;
			specialAbility = new Lightwall(this);
			specialAbility->init();
			mobility = new MultiJump();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::CLUSTERGRENADE);
			this->size = glm::vec3(0.4, 0.9, 0.4);
			break;
		case DESTROYER:
			role = 1;
			specialAbility = new Lightwall(this);
			specialAbility->init();
			mobility = new SprintD();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::OVERCHARGE);
			this->size = glm::vec3(0.8, 0.75, 0.8);
			break;
		case MOBILITY:
			role = 2;
			specialAbility = new LightSpeedSpecial();
			specialAbility->init();
			mobility = new WallJump();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::HACKINGDART);
			this->size = glm::vec3(0.8, 0.85, 0.8);
			break;
		case BRUTE:
			role = 3;
			specialAbility = new ThunderDome(this);
			specialAbility->init();
			mobility = new Dash();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::THERMITEGRENADE);
			this->size = glm::vec3(0.6, 0.85, 0.6);
			break;
		case MANIPULATOR:
			role = 4;
			specialAbility = new CleanseNovaSpecial(this);
			specialAbility->init();
			mobility = new GrapplingHookSpecial();
			mobility->init();
			consumable = new Consumable();
			consumable->init(CONSUMABLE_TYPE::VACUUMGRENADE);
			this->size = glm::vec3(0.50, 0.70, 0.50);
			break;
	}

}

float Role::getMovementSpeed()
{
	return movementSpeed;
}

void Role::update(float dt)
{
	if (role != NROFROLES)
	{
		weapons[0]->update(dt);
		weapons[1]->update(dt);
		specialAbility->update(dt);
		mobility->update(dt);

		if (swapWeaponTimer > 0)
			swapWeaponTimer -= dt;

		if (gainSpecial && specialMeter < 100.0f)
		{
			specialMeter += dt*10.0f;
			if (specialMeter >= 100.0f)
				specialMeter = 100.0f;
		}
	}
}

void Role::chooseRole(int r)
{
	if (r < NROFROLES)
	{
		role = r;
		health = atoi(loadedRoles[role][HEALTH].c_str());
		maxHealth = health;

		loadWeapons(role, 0);
		loadWeapons(role, 1);

		loadRoleSpecifics(role);

		movementSpeed = float(atof(loadedRoles[role][MOVEMENTSPEED].c_str()));
		jumpHeight = float(atof(loadedRoles[role][JUMPHEIGHT].c_str()));

		boundingBoxModifier = 0.55f;
		boundingBoxRadius = 0.9f;
		gainSpecial = true;
		currentWpn = 0;
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
	currentWpn = swapTo;
	swapWeaponTimer = 0.5f;

	if (this->role == 2 && GetSound())
	{
		if (currentWpn == 1)
		{
			GetSound()->playUserGeneratedSound(SOUNDS::soundEffectStalkerChange, CATEGORY::Guns);
		}
		else
		{
			GetSound()->playUserGeneratedSound(SOUNDS::soundEffectStalkerChangeBack, CATEGORY::Guns);
		}
		
	}
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
		health = maxHealth;
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

bool Role::getIfBusy()
{
	bool tester = false;

	if (weapons[currentWpn]->getIfReloading() || weapons[currentWpn]->getIfShooting() || swapWeaponTimer > 0)
	{
		tester = true;
	}

	return tester;
}

int Role::getRole()
{
	return role;
}

string Role::getRoleAsString()
{
	switch (role)
	{
	case ROLES::TRAPPER:		return "Trapper";
	case ROLES::DESTROYER:		return "Destroyer";
	case ROLES::MOBILITY:		return "Stalker";
	case ROLES::BRUTE:			return "Punisher";
	case ROLES::MANIPULATOR:	return "Manipulator";
	case ROLES::NROFROLES:		return "Class Select";
	}
}