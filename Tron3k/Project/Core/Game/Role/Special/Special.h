#ifndef SPECIAL_H
#define SPECIAL_H

#include "../../GameDataIndex.h"

class Special
{
	protected:
		SPECIAL_TYPE specialType;
	public:
		virtual void init() = 0;

		virtual int update(float deltaTime) = 0;

		SPECIAL_TYPE getType() { return specialType; };
};
#endif