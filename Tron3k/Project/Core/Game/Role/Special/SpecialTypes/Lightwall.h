#ifndef LIGHTWALL_H
#define LIGHTWALL_H

#include <glm/glm.hpp>
#include "../Special.h"

#include "../../../GameDataIndex.h"

class Lightwall : public Special
{
private:
	float activeTime;
public:
	Lightwall();
	~Lightwall();

	void init();
	int update(float deltaTime);
};
#endif