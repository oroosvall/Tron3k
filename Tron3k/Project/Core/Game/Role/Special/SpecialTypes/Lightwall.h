#ifndef LIGHTWALL_H
#define LIGHTWALL_H

#include <glm/glm.hpp>
#include "../Special.h"

#include "../../../GameDataIndex.h"

class Role;
class Player;

class Lightwall : public Special
{
private:
	Role* myRole = nullptr;
	bool activated = false;
public:
	Lightwall(Role* r);
	~Lightwall();

	void init();
	int update(float deltaTime);

	bool allowedToActivate(Player* p);
};
#endif