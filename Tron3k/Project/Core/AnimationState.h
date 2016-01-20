#ifndef ANIMATION_STATE_H
#define ANIMATION_STATE_H

enum AnimationState
{
	first_primary_idle,				// Trapper	| Destroyer	| Shankbot | Brute | Manipulator |
	first_primary_run, 				// Trapper	| Destroyer	| Shankbot | Brute | Manipulator |
	first_primary_fire,				// Trapper	| Destroyer	| Shankbot | Brute | Manipulator |	
	first_primary_air, 				// Trapper	| Destroyer	| Shankbot | Brute | Manipulator |
	first_primary_reload,			// Trapper	| Destroyer	| Shankbot | Brute | Manipulator |
	first_primary_throw, 			// Trapper	| use fire?	|		   | Brute | Manipulator |
	first_primary_switch,			//      	| Destroyer	| Shankbot | Brute | Manipulator |

	first_secondary_idle, 			//			| Destroyer | Shankbot | Brute | Manipulator |
	first_secondary_run,			//			| Destroyer | Shankbot | Brute | Manipulator |
	first_secondary_fire,			// Trapper	| Destroyer | Shankbot | Brute | Manipulator |
	first_secondary_air,			//			| Destroyer | Shankbot | Brute | Manipulator |
	first_secondary_reload,			//			| Destroyer |		   | Brute | Manipulator |
	first_secondary_throw, 			//			| Destroyer |          | Brute | Manipulator |
	first_secondary_switch,			//      	| Destroyer | Shankbot | Brute | Manipulator |

	third_primary_idle,				// Trapper	| Destroyer | Shankbot | Brute | Manipulator |
	third_primary_run,				// Trapper	| Destroyer | Shankbot | Brute | Manipulator |
	third_primary_run_rev,			// Trapper	| Destroyer | Shankbot | Brute | Manipulator |
	third_primary_air,				// Trapper	| idle,run? | Shankbot | Brute | Manipulator |
	third_primary_strafe_left,		// Trapper	| Destroyer | Shankbot | Brute | Manipulator |
	third_primary_strafe_right,		// Trapper	| Destroyer | Shankbot | Brute | Manipulator |
	third_primary_jump_begin,		// Trapper	|		    | Shankbot | Brute | Manipulator |
	third_primary_jump_end,			// Trapper	| Destroyer | Shankbot | Brute | Manipulator |

									//special case Brute & Manipulator need 2nd set of third animations because of their weapons
	third_secondary_idle,			//			|			|		   | Brute | Manipulator |
	third_secondary_run,			//			|			|		   | Brute | Manipulator |
	third_secondary_run_rev,		//			|			|		   | Brute | Manipulator |
	third_secondary_air,			//			|			|		   | Brute | Manipulator |
	third_secondary_strafe_left,	//			|			|		   | Brute | Manipulator |
	third_secondary_strafe_right,	//			|			|		   | Brute | Manipulator |
	third_secondary_jump_begin,		//			|			|		   | Brute | Manipulator |
	third_secondary_jump_end,		//			|			|		   | Brute | Manipulator |

									//special case Brute & Manipulator change weapon 3rd, special death animations for each weapon.
	third_primary_switch,			//			|			|		   | Brute | Manipulator |
	third_secondary_switch,			//			|			|		   | Brute | Manipulator |

									//destroyer tracks							 					   	
	third_turn_left,				//			| Destroyer |		   |       |             |
	third_turn_right,				//			| Destroyer |		   |       |             |

									//shank bot 								 						
	third_melee_standing,			//			|			| Shankbot |	   |	         |
	third_melee_run,				//			|			| Shankbot |	   |	         |
	third_shankbot_charge,			//			|			| Shankbot |	   |	         |

	third_primary_death,			// Trapper	| Destroyer | Shankbot | Brute | Manipulator |
	third_secondary_death,			//			|			|		   | Brute | Manipulator |

	none
};

// static functions can only be seen classes that includes it.
//currently included in Game, Player and RenderPipeline

int static getAnimRank(AnimationState test)
{
	switch (test)
	{
	case first_primary_idle:			return 0;
	case first_primary_run:				return 0;
	case first_primary_air:				return 0;
	case first_primary_fire:			return 1;
	case first_primary_reload:			return 1;
	case first_primary_throw:			return 1;
	case first_primary_switch:			return 1;

	case first_secondary_idle:			return 0;
	case first_secondary_run:			return 0;
	case first_secondary_air:			return 0;
	case first_secondary_fire:			return 1;
	case first_secondary_reload:		return 1;
	case first_secondary_throw:			return 1;
	case first_secondary_switch:		return 1;

	case third_primary_idle:			return 0;
	case third_primary_run:				return 0;
	case third_primary_run_rev:			return 0;
	case third_primary_air:				return 0;
	case third_primary_strafe_left:		return 0;
	case third_primary_strafe_right:	return 0;
	case third_primary_jump_begin:		return 1;
	case third_primary_jump_end:		return 1;

	case third_secondary_idle:			return 0;
	case third_secondary_run:			return 0;
	case third_secondary_run_rev:		return 0;
	case third_secondary_air:			return 0;
	case third_secondary_strafe_left:	return 0;
	case third_secondary_strafe_right:	return 0;
	case third_secondary_jump_begin:	return 1;
	case third_secondary_jump_end:		return 1;

		//destroyer tracks				
	case third_turn_left:				return 0;
	case third_turn_right:				return 0;

		//shank bot 					
	case third_shankbot_charge:			return 0;
	case third_melee_standing:			return 1;
	case third_melee_run:				return 1;

		//special case Brute & Manipula
	case third_primary_switch:			return 1;
	case third_secondary_switch:		return 1;

	case third_primary_death:			return 2;
	case third_secondary_death:			return 2;

	default:
	case none: return -1;
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