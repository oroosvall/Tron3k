#ifndef ANIMATION_STATE_H
#define ANIMATION_STATE_H

enum AnimationState
{
	//first person
	first_idle,
	first_run,
	first_air,

	first_fire,
	first_reload,

	//third person
	third_idle,
	third_run,
	third_air,

	third_jump_begin,
	third_jump_end,

	third_death
};

// static functions can only be seen classes that includes it.
//currently included in Game, Player and RenderPipeline

int static getAnimRank(AnimationState test)
{
	switch (test)
	{
	case first_idle:		return 0;
	case first_run:			return 0;
	case first_air:			return 0;

	case first_fire:		return 1;
	case first_reload:		return 1;

	case third_idle:		return 0;
	case third_run:			return 0;
	case third_air:			return 0;

	case third_jump_begin:	return 1;
	case third_jump_end:	return 1;

	case third_death:		return 2;

	default:				return -1;
	}
};

bool static checkAnimOverwrite(AnimationState current, AnimationState test)
{
	//dont cancle animation if it's the same animation
	if (current == test)
		return false;

	//a higher anim index always cancles out current anim
	if (current < test)
		return true;

	// if test is smalled than current we have to compare ranks because it may still cancle out current
	if (getAnimRank(current) <= getAnimRank(test))
		return true;

	return false;
};

#endif