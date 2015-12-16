#ifndef ANIM_MANAGER_H
#define ANIM_MANAGER_H

#include "../Project/Core/AnimationState.h"

class AnimManager 
{

public:
	
	struct animState
	{
		AnimationState state;
		unsigned int frame;
		unsigned int frameEnd;
		float speed;
		float timepass;
		float timeLength;

		animState()
		{
			state = AnimationState::none;
			frame = 0;
			speed = 1.0f / 30.0f;
			frameEnd = 0;
			timepass = 0;
			timeLength = 0;
		}
	};

	//save states for all players
	animState animStates[20];

	void updateAnimStates(int playerID, int role, AnimationState current, float dt);
	
private:
	void setAnim(animState& current, AnimationState overide);
};

#endif