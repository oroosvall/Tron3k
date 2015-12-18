#include "AnimManager.h"

void AnimManager::updateAnimStates(int playerID, int role, AnimationState current, float dt)
{
	bool overide = checkAnimOverwrite(animStates[playerID].state, current);

	if (overide) // replace animation with the new one
	{
		setAnim(animStates[playerID], current);
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
				animStates[playerID] = animState();
			}
			//freeze rank 2 anims
			else if( rank == 2)
			{
				animStates[playerID].timepass = animStates[playerID].timeLength;
			}
		}

		float index = animStates[playerID].timepass / animStates[playerID].timeLength;

		//if we should run the animation backwards
		if(animStates[playerID].state == AnimationState::third_run_rev)
			animStates[playerID].frame = animStates[playerID].frameEnd - animStates[playerID].frameEnd * index;
		else
			animStates[playerID].frame = animStates[playerID].frameEnd * index;
	}
}

void AnimManager::setAnim(animState& current, AnimationState overide)
{
	current = animState();
	current.state = overide;

	switch (overide)
	{
	case first_idle:		current.frameEnd = 0;		break;
	case first_run:			current.frameEnd = 21;		break;
	case first_air:			current.frameEnd = 10;		break;
	case first_fire:		current.frameEnd = 10;		break;
	case first_reload:		current.frameEnd = 130;		break;
	case third_idle:		current.frameEnd = 42;		break;
	case third_run:			current.frameEnd = 21;		break;
	case third_run_rev:		current.frameEnd = 21;		break;
	case third_strafe_left: current.frameEnd = 21;		break;
	case third_strafe_right:current.frameEnd = 21;		break;
	case third_air:			current.frameEnd = 121;		break;
	case third_jump_begin:	current.frameEnd = 9;		break;
	case third_jump_end:	current.frameEnd = 9;		break;
	case third_death:		current.frameEnd = 10;		break;
	default:				current = animState();		break;
	}

	current.timeLength = current.frameEnd * current.speed;
}