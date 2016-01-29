#include "Effect.h"

bool Effect::thisPlayerHasBeenHitByMe(int conid)
{
	for (int c = 0; c < playersHitByMe.size(); c++)
	{
		if (playersHitByMe[c] == conid)
			return true;
	}
	return false;
}