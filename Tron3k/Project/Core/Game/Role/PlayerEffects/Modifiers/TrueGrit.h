#ifndef TRUEGRIT_H
#define TRUEGRIT_H

#include "../Modifier.h"



class TrueGrit : public Modifier
{
private:
	glm::vec3 vel;
	float healDelay = 0.1;
	float healDuration = 1.5;
	int oldHealth;
	int damageTaken;
	bool dashDone = false;
	float timer = healDelay;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
	bool isDashDone() { return dashDone; };
	void refresh();
};
#endif