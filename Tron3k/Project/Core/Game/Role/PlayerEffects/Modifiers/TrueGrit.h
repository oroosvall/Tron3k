#ifndef TRUEGRIT_H
#define TRUEGRIT_H

#include "../Modifier.h"

class TrueGrit : public Modifier
{
private:
	int oldHealth;
	int damageTaken;
public:
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif