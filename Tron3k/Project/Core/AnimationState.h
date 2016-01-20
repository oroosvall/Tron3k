#ifndef ANIMATION_STATE_H
#define ANIMATION_STATE_H

enum AnimationState
{
	//first person
	first_primary_idle, //does not exist yet
	first_primary_run, //new
	first_primary_air, //does not exist yet

	first_primary_fire, //new
	first_primary_reload,
	first_primary_throw, //new
	first_primary_switch, //new

	first_secondary_fire, //new

	//third person
	third_idle,
	third_run,
	third_run_rev,
	third_air,
	third_strafe_left,
	third_strafe_right,

	third_jump_begin,
	third_jump_end,

	third_death,

	none,

	iteration
};

// static functions can only be seen classes that includes it.
//currently included in Game, Player and RenderPipeline

int static getAnimRank(AnimationState test)
{
	switch (test)
	{
	case first_primary_idle:		return 0;
	case first_primary_run:			return 0;
	case first_primary_air:			return 0;

	case first_primary_fire:		return 1;
	case first_primary_reload:		return 1;
	case first_primary_throw:		return 1;
	case first_primary_switch:		return 1;

	case first_secondary_fire:		return 1;

	case third_idle:		return 0;
	case third_run:			return 0;
	case third_run_rev:		return 0;
	case third_air:			return 0;

	case third_strafe_left:	return 0;
	case third_strafe_right:return 0;

	case third_jump_begin:	return 1;
	case third_jump_end:	return 1;

	case third_death:		return 2;

	default:				return -1;
	}
};

bool static checkAnimOverwrite(AnimationState current, AnimationState test)
{
	if (current == test)
	{
		//idle, run, air, death dont cancel if compared to themselves
		if(getAnimRank(current) == 0 || getAnimRank(current) == 2)
			return false;
		//reload, jumpend and other rank 1 do cancel out themselves
		return true;
	}
		

	//a higher anim index always cancles out current anim
	if (current < test)
		return true;

	// if test is smalled than current we have to compare ranks because it may still cancle out current
	if (getAnimRank(current) <= getAnimRank(test))
		return true;

	return false;
};



#endif