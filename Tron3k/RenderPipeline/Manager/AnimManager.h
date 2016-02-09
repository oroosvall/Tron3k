#ifndef ANIM_MANAGER_H
#define ANIM_MANAGER_H

#include "../../Project/Core/AnimationState.h"

class AnimManager 
{

public:
	
	int keyFrameLenghts[5 * AnimationState::none];

	struct animState
	{
		AnimationState state;
		int role;
		unsigned int frame;
		unsigned int frameEnd;
		float speed;
		float timepass;
		float timeLength;
		bool timeout;
		bool firstPerson;

		animState()
		{
			state = AnimationState::none;
			role = 0;
			frame = 0;
			speed = 1.0f / 30.0f;
			frameEnd = 0;
			timepass = 0;
			timeLength = 0;
			timeout = false;
			firstPerson = false;
		}
	};

	//save states for all players
	animState animStates[20];

	void updateAnimStates(int playerID, int role, AnimationState current, float dt, bool firstPerson);
	
private:
	void setAnim(animState& current, AnimationState overide, bool firstPerson);
};

#endif