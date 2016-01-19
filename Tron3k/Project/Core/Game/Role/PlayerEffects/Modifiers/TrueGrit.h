#ifndef TRUEGRIT_H
#define TRUEGRIT_H

#include "../Modifier.h"



class TrueGrit : public Modifier
{
private:
	float healDelay = 0.1;
	int oldHealth;
	int damageTaken;
	bool dashDone = false;
	float endTime;
	float timer = healDelay;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif