#ifndef HACKINGDARTSPECIAL_H
#define HACKINGDARTSPECIAL_H

#include <glm/glm.hpp>
#include "../Special.h"

#include "../../../GameDataIndex.h"

class Role;
class Player;

class HackingDartSpecial : public Special
{
private:
	Role* myRole = nullptr;
public:
	HackingDartSpecial(Role* r);
	~HackingDartSpecial();

	void init();
	int update(float deltaTime);

	bool allowedToActivate(Player* p);
};
#endif