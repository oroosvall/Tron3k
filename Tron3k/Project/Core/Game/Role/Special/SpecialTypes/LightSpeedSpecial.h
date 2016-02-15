#ifndef LIGHTSPEEDSPECIAL_H
#define LIGHTSPEEDSPECIAL_H

#include "../Special.h"

class Role;
class Player;

class LightSpeedSpecial : public Special
{
private:
public:
	void init();
	int update(float deltaTime);
	bool allowedToActivate(Player* p);
};

#endif