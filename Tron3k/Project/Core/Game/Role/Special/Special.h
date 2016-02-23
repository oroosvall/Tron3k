#ifndef SPECIAL_H
#define SPECIAL_H

#include "../../GameDataIndex.h"

class Player;

class Special
{
	protected:
		SPECIAL_TYPE specialType;
		int specialId = -1;
		int activationCost;
		bool isOnJumpKey = false;
	public:
		virtual void init() = 0;

		virtual int update(float deltaTime) = 0;

		SPECIAL_TYPE getType() { return specialType; };

		int getSpecialId();

		virtual bool allowedToActivate(Player* p) = 0;
		bool isThisOnJumpKey() { return isOnJumpKey; };

		int getActivationCost() { return activationCost; };
};
#endif