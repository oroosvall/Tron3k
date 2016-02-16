#ifndef GRAPPLINGHOOKSPECIAL_H
#define GRAPPLINGHOOKSPECIAL_H

#include <glm/glm.hpp>
#include "../Special.h"

#include "../../../GameDataIndex.h"

class Role;
class Player;

class GrapplingHookSpecial : public Special
{
private:
	Role* myRole = nullptr;
	float cooldown = 0.0f;
public:
	GrapplingHookSpecial();
	~GrapplingHookSpecial();

	void init();
	int update(float deltaTime);

	bool allowedToActivate(Player* p);
};
#endif