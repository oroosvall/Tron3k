#ifndef ROLE_H
#define ROLE_H

#include "Weapon\weapon.h"
#include<string>

using namespace std;

enum PLAYERMSG { TRAPPER, DESTROYER, MOBILITY, BRUTE, MANIPULATOR };

class Role
{
	private:
		int currentWpn = 0;
		string roles[5];

		Weapon weapons[2];
		Weapon pickup;
		Weapon super;

	public:
		Role();
		~Role();

		void chooseRole(int role);
};
#endif