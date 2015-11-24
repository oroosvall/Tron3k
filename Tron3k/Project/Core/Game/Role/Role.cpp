#include "Role.h"

Role::Role()
{
	currentWpn = 0;
	health = 1;

	roles[TRAPPER] = "GameFiles/Roles/trapper.txt";
	roles[DESTROYER] = "GameFiles/Roles/destroyer.txt";
	roles[MOBILITY] = "GameFiles/Roles/mobility.txt";
	roles[BRUTE] = "GameFiles/Roles/brute.txt";
	roles[MANIPULATOR] = "GameFiles/Roles/manipulator.txt";

	ifstream roleFile;
	for (int i = 0; i < NROFROLES; i++)
	{
		for (int y = 0; y < NROFREAD; y += 5)
		{
			roleFile.open(roles[i]);

			getline(roleFile, loadedRoles[i][y]);
			getline(roleFile, loadedRoles[i][y + 1]);
			getline(roleFile, loadedRoles[i][y + 2]);
			getline(roleFile, loadedRoles[i][y + 3]);
			getline(roleFile, loadedRoles[i][y + 4]);
			roleFile.close();
		}
	}

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
	stringstream temp;
	if (role < NROFROLES)
	{
		health = atoi(loadedRoles[role][HEALTH].c_str());

		weapons[0].init(36, (WEAPON_TYPE)atoi(loadedRoles[role][MAINWEP].c_str()), 0.5);
		weapons[1].init(80, (WEAPON_TYPE)atoi(loadedRoles[role][SECWEP].c_str()), 0.1);

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