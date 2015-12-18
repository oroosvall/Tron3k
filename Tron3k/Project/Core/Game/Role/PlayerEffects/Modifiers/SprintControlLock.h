#ifndef SPRINTCONTROLLOCK_H
#define SPRINTCONTROLLOCK_H

#include "../Modifier.h"

class SprintControlLock : public Modifier
{
private:
	float sprintSpeed;
	glm::vec3 vel;
	float maxDuration = 5.0f;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif