#ifndef CLEANSESPECIAL_H
#define CLEANSESPECIAL_H

#include <glm/glm.hpp>
#include "../Special.h"

#include "../../../GameDataIndex.h"

class Role;
class Player;

class CleanseNovaSpecial : public Special
{
private:
	Role* myRole = nullptr;
public:
	CleanseNovaSpecial(Role* r);
	~CleanseNovaSpecial();

	void init();
	int update(float deltaTime);

	bool allowedToActivate(Player* p);
};
#endif