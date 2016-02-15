#include "AnimManager.h"

void AnimManager::updateAnimStates(int playerID, int role, AnimationState current, float dt, bool firstPerson)
{
	bool overide = checkAnimOverwrite(animStates[playerID].state, current);

	if (animStates[playerID].timeout)
		overide = true;

	if (animStates[playerID].role != role)
	{
		overide = true;
		animStates[playerID].role = role;
		animStates[playerID].state = AnimationState::none;
	}

	if (getAnimRank(animStates[playerID].state) == 2 && current == AnimationState::none)
		overide = true;

	if (animStates[playerID].firstPerson != firstPerson)
		overide = true;

	if (overide) // replace animation with the new one
	{
		setAnim(animStates[playerID], current, firstPerson);
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
		//if (animStates[playerID].state == AnimationState::third_primary_run_rev)
		//	animStates[playerID].frame = (int)animStates[playerID].frameEnd - animStates[playerID].frameEnd * index;
		//else
		{
			animStates[playerID].frame = (int)animStates[playerID].frameEnd * index;
			if (animStates[playerID].frame >= animStates[playerID].frameEnd)
				if(rank != 2)
					animStates[playerID].frame = 0;
				else
					animStates[playerID].frame = animStates[playerID].frameEnd-1;
		}
		if (animStates[playerID].timeout)
			animStates[playerID].frame = animStates[playerID].frameEnd-1;
	}

}

void AnimManager::setAnim(animState& current, AnimationState overide, bool firstPerson)
{
	if (keyFrameLenghts[current.role*AnimationState::none + overide] == 0)
	{
		if (overide != AnimationState::none)
		{
			current.state = AnimationState::none;
			current.timeLength = 0;
			return;
		}
	}
	int role = current.role;
	current = animState();
	current.state = overide;
	current.role = role;

	current.firstPerson = firstPerson;

	current.frameEnd = keyFrameLenghts[current.role*AnimationState::none + current.state];

	if (overide == AnimationState::first_primary_switch ||
		overide == AnimationState::first_primary_switch_IN ||
		overide == AnimationState::first_secondary_switch ||
		overide == AnimationState::first_secondary_switch_IN)
	{
		current.speed *= 0.5f;
	}

	current.timeLength = current.frameEnd * current.speed;
}