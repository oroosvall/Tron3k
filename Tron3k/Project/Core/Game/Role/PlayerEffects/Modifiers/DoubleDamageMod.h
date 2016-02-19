#ifndef DOUBLEDAMAGEMOD_H
#define DOUBLEDAMAGEMOD_H

#include "../Modifier.h"

class DoubleDamageMod : public Modifier
{
private:
public:
	~DoubleDamageMod();
	void init(Player* myTarget);
	int getData(float dt);
	int setData(float dt);
};
#endif