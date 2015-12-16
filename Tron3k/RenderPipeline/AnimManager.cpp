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
		float index = animStates[playerID].timepass / animStates[playerID].timeLength;
		animStates[playerID].frame = animStates[playerID].frameEnd * index;
	}
}

void AnimManager::setAnim(animState& current, AnimationState overide)
{
	current = animState();
	current.state = overide;

	switch (overide)
	{
	case first_idle:		current.frameEnd = 10;		break;
	case first_run:			current.frameEnd = 21;		break;
	case first_air:			current.frameEnd = 10;		break;
	case first_fire:		current.frameEnd = 10;		break;
	case first_reload:		current.frameEnd = 130;		break;
	case third_idle:		current.frameEnd = 10;		break;
	case third_run:			current.frameEnd = 10;		break;
	case third_air:			current.frameEnd = 10;		break;
	case third_jump_begin:	current.frameEnd = 10;		break;
	case third_jump_end:	current.frameEnd = 10;		break;
	case third_death:		current.frameEnd = 10;		break;
	default:				current = animState();		break;
	}

	current.timeLength = current.frameEnd * current.speed;
}