#ifndef ANIMATION_STATE_H
#define ANIMATION_STATE_H

enum AnimationState
{
	first_primary_idle,				// Trapper	| Destroyer	| Shankbot | Brute | Manipulator |
	first_primary_run, 				// Trapper	| Destroyer	| Shankbot | Brute | Manipulator |
	first_primary_fire,				// Trapper	| Destroyer	| Shankbot | Brute | Manipulator |	
	first_primary_reload,			// Trapper	| Destroyer	| Shankbot | Brute | Manipulator |
	first_primary_throw, 			// Trapper	| use fire?	|		   | Brute | Manipulator |
	first_primary_switch,			//      	| Destroyer	| Shankbot | Brute | Manipulator |
	first_primary_switch_IN,		//      	|           |          | Brute | Manipulator |

	first_secondary_idle, 			//			| Destroyer | Shankbot | Brute | Manipulator |
	first_secondary_run,			//			| Destroyer | Shankbot | Brute | Manipulator |
	first_secondary_fire,			// Trapper	| Destroyer | Shankbot | Brute | Manipulator |
	first_secondary_reload,			//			| Destroyer |		   | Brute | Manipulator |
	first_secondary_throw, 			//			| Destroyer |          | Brute | Manipulator |
	first_secondary_switch,			//      	| Destroyer | Shankbot | Brute | Manipulator |
	first_secondary_switch_IN,		//      	|           |          | Brute | Manipulator |

	//manip
	first_secondary_idle_left,		// 			|			|		   |	   | Manipulator |
	first_secondary_run_left,		// 			|			|		   |	   | Manipulator |
	first_secondary_fire_left,		// 			|			|		   |	   | Manipulator |
	first_secondary_death_left,		// 			|			|		   |	   | Manipulator |



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

									//destroyer only							 					   	
	third_turn_left,				//			| Destroyer |		   |       |             |
	third_turn_right,				//			| Destroyer |		   |       |             |

									//brute only
	third_brute_primary_dash,		//			|			|		   | Brute |			 |
	third_brute_secondary_dash,		//			|			|		   | Brute |			 |

									//manipulator only
	third_manipulator_pounce,		//			|			|		   |	   | Manipulator |

									//shank bot only								 						
	third_shankbot_melee_standing,	//			|			| Shankbot |	   |	         |
	third_shankbot_melee_run,		//			|			| Shankbot |	   |	         |
	third_shankbot_charge,			//			|			| Shankbot |	   |	         |
	third_shankbot_walljump_right,	//			|			| Shankbot |	   |	         |
	third_shankbot_walljump_left,	//			|			| Shankbot |	   |	         |

	third_primary_death,			// Trapper	| Destroyer | Shankbot | Brute | Manipulator |
	third_secondary_death,			//			|			|		   | Brute | Manipulator |

	first_primary_death,			// ?	| ? | ? | ? | ? |
	first_secondary_death,			// ?	| ? | ? | ? | ? |

	dance,

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
	case first_primary_fire:			return 1;
	case first_primary_reload:			return 1;
	case first_primary_throw:			return 1;
	case first_primary_switch:			return 1;
	case first_primary_switch_IN:		return 1;

	case first_secondary_idle:			return 0;
	case first_secondary_run:			return 0;
	case first_secondary_fire:			return 1;
	case first_secondary_reload:		return 1;
	case first_secondary_throw:			return 1;
	case first_secondary_switch:		return 1;
	case first_secondary_switch_IN:		return 1;

	case first_secondary_idle_left:		return 0;
	case first_secondary_run_left:		return 0;
	case first_secondary_fire_left:		return 1;
	case first_secondary_death_left:	return 2;
		
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

		//brute 
	case third_brute_primary_dash:		return 1;
	case third_brute_secondary_dash:	return 1;

		//manipulator
	case third_manipulator_pounce:		return 1;

		//shank bot 					
	case third_shankbot_charge:			return 0;
	case third_shankbot_melee_standing:	return 1;
	case third_shankbot_melee_run:		return 1;
	case third_shankbot_walljump_right: return 1;
	case third_shankbot_walljump_left:  return 1;

		//special case Brute & Manipula
	case third_primary_switch:			return 1;
	case third_secondary_switch:		return 1;

	case third_primary_death:			return 2;
	case third_secondary_death:			return 2;
	case first_primary_death:			return 2;
	case first_secondary_death:			return 2;

	case dance:							return 0;

	default:
	case none: return -1;
	}
};

bool static checkAnimOverwrite(AnimationState current, AnimationState test)
{
	if (getAnimRank(current) == 2 && getAnimRank(test) == -1)
		return true;

	if (current == test)
	{
		//idle, run, air, death dont cancel if compared to themselves
		if(getAnimRank(current) == 0 || getAnimRank(current) == 2)
			return false;
		//reload, jumpend and other rank 1 do cancel out themselves
		return true;
	}
		
	// if test is smalled than current we have to compare ranks because it may still cancle out current
	if (getAnimRank(current) <= getAnimRank(test))
		return true;

	return false;
};

void static animOverideIfPriority(AnimationState& current, AnimationState test)
{
	if (current == AnimationState::none)
		return;
	if (checkAnimOverwrite(current, test))
		current = test;
}


#endif