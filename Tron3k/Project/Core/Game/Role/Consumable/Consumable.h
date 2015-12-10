#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "../../GameDataIndex.h"

class Consumable
{
private:
	CONSUMABLE_TYPE type;
	bool used = false;
public:
	Consumable();
	void init(CONSUMABLE_TYPE ct);
	~Consumable();

	bool use();
	void reset() { used = false; };

	CONSUMABLE_TYPE getType() { return type; };
};
#endif