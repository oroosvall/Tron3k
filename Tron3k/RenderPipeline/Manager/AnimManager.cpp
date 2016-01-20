#include "AnimManager.h"

void AnimManager::updateAnimStates(int playerID, int role, AnimationState current, float dt)
{
	bool overide = checkAnimOverwrite(animStates[playerID].state, current);
	if (animStates[playerID].timeout)
	{
		overide = true;
	}

	if (current == AnimationState::third_run)
	{
		int smt = 1;
	}

	if (overide) // replace animation with the new one
	{
		setAnim(animStates[playerID], current);
		animStates[playerID].timeout = false;
	}
	else
	{
		animStates[playerID].timepass += dt;

		int rank = getAnimRank(animStates[playerID].state);

		//If the animation ended
		if (animStates[playerID].timepass > animStates[playerID].timeLength)
		{
			//loop rank 0 anims
			if (rank == 0)
			{
				animStates[playerID].timepass -= animStates[playerID].timeLength;
			}
			//fallbank rank 1 anims
			else if (rank == 1)
			{
				//animStates[playerID] = animState();
				animStates[playerID].timeout = true;
			}
			//freeze rank 2 anims
			else if (rank == 2)
			{
				animStates[playerID].timepass = animStates[playerID].timeLength;
			}
		}

		float index = animStates[playerID].timepass / animStates[playerID].timeLength;

		//if we should run the animation backwards
		if (animStates[playerID].state == AnimationState::third_run_rev)
			animStates[playerID].frame = animStates[playerID].frameEnd - animStates[playerID].frameEnd * index;
		else
		{
			animStates[playerID].frame = animStates[playerID].frameEnd * index;
			if (animStates[playerID].frame >= animStates[playerID].frameEnd && rank != 2)
				animStates[playerID].frame = 0;
			else
				animStates[playerID].frame = animStates[playerID].frameEnd-1;
		}
		if (animStates[playerID].timeout)
			animStates[playerID].frame = animStates[playerID].frameEnd-1;
	}
}

void AnimManager::setAnim(animState& current, AnimationState overide)
{
	if (keyFrameLenghts[current.role*AnimationState::none + overide] == 0)
	{
		return;
	}
	current = animState();
	current.state = overide;


	current.frameEnd = keyFrameLenghts[current.role*AnimationState::none + current.state];

	current.timeLength = current.frameEnd * current.speed;
}