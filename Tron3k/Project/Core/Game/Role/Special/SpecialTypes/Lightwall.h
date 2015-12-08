#ifndef LIGHTWALL_H
#define LIGHTWALL_H

#include <glm/glm.hpp>
#include "../Special.h"

#include "../../../GameDataIndex.h"

class Lightwall : public Special
{
private:
	bool activated = false;
public:
	Lightwall();
	~Lightwall();

	void init();
	int update(float deltaTime);

	bool allowedToActivate(Player* p);
};
#endif