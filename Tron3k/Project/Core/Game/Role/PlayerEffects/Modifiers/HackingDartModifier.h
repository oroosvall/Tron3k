#ifndef HACKINGDARTMODIFIER_H
#define HACKINGDARTMODIFIER_H

#include "../Modifier.h"

class HackingDartModifier : public Modifier
{
private:
	float timer = 10.0f;
	glm::vec3 vel;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif