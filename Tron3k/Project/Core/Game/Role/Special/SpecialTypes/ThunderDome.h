#ifndef THUNDERDOME_H
#define THUNDERDOME_H

#include <glm/glm.hpp>
#include "../Special.h"

#include "../../../GameDataIndex.h"

class Role;
class Player;

class ThunderDome : public Special
{
private:
	Role* myRole = nullptr;
	bool activated = false;
public:
	ThunderDome(Role* r);
	~ThunderDome();

	void init();
	int update(float deltaTime);

	bool allowedToActivate(Player* p);
};
#endif