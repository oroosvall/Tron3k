#include "Player.h"

Player::Player()
{

}

Player::~Player()
{
	for (unsigned int c = 0; c < myModifiers.size(); c++)
		delete myModifiers[c];
}

void Player::init(std::string pName, glm::vec3 initPos, bool isLocal)
{
	//physics = phy;
	cam = CameraInput::getCam();
	i = Input::getInput();

	name = pName;
	pos = initPos;
	vel = glm::vec3(0, 0, 0);
	dir = vec3(0, 0, -1);
	//dir = cam->getDir();

	anim_first_current = AnimationState::first_primary_idle;
	anim_first_framePeak = AnimationState::first_primary_idle;
	anim_third_current = AnimationState::third_primary_idle;
	anim_third_framePeak = AnimationState::third_primary_idle;

	isLocalPlayer = isLocal;

	rotate(0, -3.141592654f, 0);

	nameChanged = true;
}

void Player::setName(std::string newName)
{
	name = newName;
	nameChanged = true;
}

void Player::setGoalPos(glm::vec3 newPos)
{
	goalpos = newPos;
	oldPos = pos;
	goalTimer = 0.0f;
}

void Player::setGoalDir(glm::vec3 newDir)
{
	goaldir = newDir;
	oldDir = dir;
}

void Player::setCollisionInfo(std::vector<glm::vec4> collNormals)
{
	if (collNormals.size() < 20 - collisionNormalSize)
	{
		for (int i = 0; i < collNormals.size(); i++)
		{
			collisionNormals[i + collisionNormalSize] = collNormals[i];
		}

		collisionNormalSize += collNormals.size();
	}
}

void Player::setExplodingInfo(std::vector<glm::vec4> expDirs)
{
	if (expDirs.size() > 0)
	{
		//pos += normalize(vec3(expDirs[0])) / expDirs[0].w;
		vel *= normalize(glm::vec3(expDirs[0].x, expDirs[0].y, expDirs[0].z));// *expDirs[0].w;
		vel.y = +5.0f; //Jump height at initial dash
		grounded = false;
	}
}

void Player::movePlayer(float dt, glm::vec3 oldDir)
{
	if (!this->getFootsteps())
	{
		this->footstepsLoopReset(dt);
	}

	vec3 oldPos = pos;
	glm::vec3 playerVel = vec3(0);
	playerVel.x = vel.x*role.getMovementSpeed();
	playerVel.z = vel.z*role.getMovementSpeed();
	playerVel.y = vel.y;
	pos += playerVel * dt; //Here we will also include external forces.. EDIT: External forces moved, for now
	vec3 posadjust = vec3(0);

	if (GetSoundActivated())
	{
		GetSound()->setLocalPlayerPos(cam->getPos());
		GetSound()->setLocalPlayerDir(cam->getDir());
	}

	if ((vel.x != 0 || vel.z != 0) && this->grounded)
	{

		if (this->getFootsteps() && this->getGrounded() && GetSoundActivated() && this->role.getRole() != 1 && !i->getKeyInfo(controls.jump))
		{
			this->setFootstepsCountdown();
			this->setFootstepsLoop(false);
			GetSound()->PlayStereoFootsteps(this->role.getRole(), pos.x, pos.y, pos.z);
		}
	}

	else
	{
		if (this->role.getRole() == 1 && GetSoundActivated() && GetSound()->destroyerPaused == false)
		{
			GetSound()->stopDestroyer(pos.x, pos.y, pos.z);
		}

		else if (this->role.getRole() == 3 && GetSoundActivated() && GetSound()->brutePaused == false)
		{
			GetSound()->setBruteLoopFalse();
			GetSound()->brutePaused = true;

		}
	}
}

void Player::movePlayerCollided(float dt, glm::vec3 oldDir)
{
	//effectCollisionHandling();
	//Collision handling here, after movement
	bool ceiling = false;
	vec3 posadjust = vec3(0);

	//if we collided with something
	int * collS = &collisionNormalSize;
	if (collisionNormalSize > 0)
	{
		collided = true;

		bool ceiling = false;
		grounded = false;
		int xDivs = 0;
		int yDivs = 0;
		int zDivs = 0;

		vec3 pd;
		float l = 0;
		int divs = 0;
		for (int k = 0; k < collisionNormalSize; k++)
		{
			//push pos away and lower velocity using pendepth
			vec3 pendepth = vec3(collisionNormals[k]) * collisionNormals[k].w;
			if (collisionNormals[k].y < -0.6f)
				ceiling = true;

			//ramp factor and grounded
			if (collisionNormals[k].y > 0.4f)
			{
				grounded = true;
			}

			
			if (abs(pendepth.x) > abs(posadjust.x))
			{
				posadjust.x += pendepth.x;
				xDivs++;
			}
			if (abs(pendepth.y) > abs(posadjust.y))
			{
				posadjust.y += pendepth.y;
				yDivs++;
			}
			if (abs(pendepth.z) > abs(posadjust.z))
			{
				posadjust.z += pendepth.z;
				zDivs++;
			}
		}

		if (xDivs > 1)
			posadjust.x /= xDivs;
		if (yDivs > 1)
			posadjust.y /= yDivs;
		if (zDivs > 1)
			posadjust.z /= zDivs;

		posadjust = posadjust * 0.99f;
		
		vel += posadjust;
		if (ceiling)
		{
			posadjust.y = 0;
			grounded = false;
		}

		pos += posadjust;

		if (posadjust.y < 0.00001f && posadjust.y > -0.00001f)
		{
			vel.y -= posadjust.y;
			pos.y -= posadjust.y;
		}

		if (ceiling && vel.y > 0)
			vel.y = 0;

		
	}
	else
	{
		collided = false;
		airVelocity = vel;
		grounded = false;
	}
}

void Player::effectCollisionHandling()
{
	//get normals
	//std::vector<vec4> cNorms = physics->sphereVWorldCollision(pos - (vec3(0, 0.55f, 0)), 1);

	//Collision handling here, after movement
	bool ceiling = false;
	vec3 posadjust = vec3(0);

	//if we collided with something
	if (effectCollNormalSize > 0)
	{
		collided = true;

		bool ceiling = false;
		grounded = false;
		int xDivs = 0;
		int yDivs = 0;
		int zDivs = 0;
		for (int k = 0; k < effectCollNormalSize; k++)
		{
			//push pos away and lower velocity using pendepth
			vec3 pendepth = vec3(effectCollisionNormals[k]) * effectCollisionNormals[k].w;
			if (effectCollisionNormals[k].y < -0.6f)
				ceiling = true;

			//ramp factor and grounded

			if (effectCollisionNormals[k].y > 0.4f)
			{
				grounded = true;
			}

			// abslut value, if two collisions from the same angle they should not move us twice the distance
			//if (length(pendepth) > length(posadjust))
			//posadjust = pendepth;

			if (pendepth.x + FLT_EPSILON > 0.0f || pendepth.x - FLT_EPSILON < 0.0f)//abs(posadjust.x) < abs(pendepth.x))
			{
			posadjust.x += pendepth.x;
			xDivs++;
			}
			if (pendepth.y + FLT_EPSILON > 0.0f || pendepth.y - FLT_EPSILON < 0.0f)
			{
			posadjust.y += pendepth.y;
			yDivs++;
			}
			if (pendepth.z + FLT_EPSILON > 0.0f || pendepth.z - FLT_EPSILON < 0.0f)
			{
			posadjust.z += pendepth.z;
			zDivs++;
			}/*
			if ((posadjust.y * posadjust.y) / k < pendepth.y * pendepth.y)
			posadjust.y += pendepth.y;
			if (abs(posadjust.z) < abs(pendepth.z))
			posadjust.z += pendepth.z;
			*/
			//posadjust += pendepth;
		}

		if (xDivs > 1)
			posadjust.x /= xDivs;
		if (yDivs > 1)
			posadjust.y /= yDivs;
		if (zDivs > 1)
			posadjust.z /= zDivs;
		//if (posadjust.y < 0)
		//ceiling = true;
		//if (posadjust.y > 0.4)
		//	grounded = true;
		/*posadjust = normalize(posadjust);// /= collisionNormalSize;
		for (int k = 0; k < collisionNormalSize; k++)
		{
		posadjust *= collisionNormals[k].w;
		}*/
		// this is for air only since grounded will set the vel to 0 later
		// the dt * 0.5 is supposed to remove almost all velocity in that dir
		// while + posajust w/o  /dt  will remove it slower
		posadjust = posadjust * 0.99f;


		if (ceiling)
		{
			posadjust.y = 0;
			//grounded = false;
		}
		vel += posadjust;// / dt * 0.5f;
		pos += posadjust;

		if (posadjust.y < 0.00001f && posadjust.y > -0.00001f)
		{
			vel.y -= posadjust.y;
			pos.y -= posadjust.y;
		}

		if (ceiling && vel.y > 0)
			vel.y = 0;
	}
}

void Player::footstepsLoopReset(float dt)
{
	if (footstepsCountdown > FLT_EPSILON)
	{
		footstepsCountdown -= dt;
	}

	else
	{
		footstepsLoop = true;
	}
}

bool Player::getFootsteps()
{
	return this->footstepsLoop;
}

void Player::modifiersGetData(float dt)
{
	for (unsigned int c = 0; c < myModifiers.size(); c++)
	{
		int msg = myModifiers[c]->getData(dt);
		if (msg == 1)
		{
			delete myModifiers[c];
			myModifiers[c] = myModifiers[myModifiers.size() - 1];
			myModifiers.pop_back();
			c--;
		}
	}
}

void Player::setFootstepsCountdown()
{
	if (this->role.getRole() == 2)
	{
		this->footstepsCountdown = 0.2;
	}
	else if (this->role.getRole() == 4)
	{
		this->footstepsCountdown = 0.39;
	}
	else if (this->role.getRole() == 1)
	{
		this->footstepsCountdown = 0.7;
	}

	else if (this->role.getRole() == 3)
	{
		this->footstepsCountdown = 0.55;
	}
	else
	{
		this->footstepsCountdown = 0.7;
	}

}

void Player::setFootstepsLoop(bool theBool)
{
	this->footstepsLoop = theBool;
}

void Player::modifiersSetData(float dt)
{
	for (unsigned int c = 0; c < myModifiers.size(); c++)
	{
		int msg = myModifiers[c]->setData(dt);
		if (msg == 1)
		{
			delete myModifiers[c];
			myModifiers[c] = myModifiers[myModifiers.size() - 1];
			myModifiers.pop_back();
			c--;
		}
	}
}

void Player::cleanseModifiers(bool stickies)
{
	for (int c = myModifiers.size() - 1; c >= 0; c--)
	{
		if (!stickies)
		{
			if (!myModifiers[c]->isSticky())
			{
				delete myModifiers[c];
				myModifiers[c] = myModifiers[myModifiers.size() - 1];
				myModifiers.pop_back();
			}
		}
		else
		{
			delete myModifiers[c];
			myModifiers[c] = myModifiers[myModifiers.size() - 1];
			myModifiers.pop_back();
		}
	}
}

bool Player::removeSpecificModifier(MODIFIER_TYPE mt)
{
	for (unsigned int c = 0; c < myModifiers.size(); c++)
	{
		if (myModifiers[c]->getType() == mt)
		{
			delete myModifiers[c];
			myModifiers[c] = myModifiers[myModifiers.size() - 1];
			myModifiers.pop_back();
			return true;
		}
	}
	return false;
}

PLAYERMSG Player::update(float dt, bool freecam, bool spectatingThisPlayer, bool spectating)
{
	diedThisFrame = false;
	PLAYERMSG msg = NONE;

	if (isDead)
		deathTimer += dt;
	else
		deathTimer = 0;


	if (role.getRole() != NROFROLES || spectating)
	{
		modifiersGetData(dt); //Dont Remove Please!

		vec3 olddir = cam->getDir();
		if (isLocalPlayer) // even if we are the local player we can be dead and spectating some one
		{
			role.update(dt);
			//default movement anims
			animRole = role.getRole();
			animPrimary = false;
			if (role.getWeaponNRequiped() == 0) // if primary
				animPrimary = true;

			// *** first person defaults ***
			anim_first_current = AnimationState::first_primary_idle;
			if (animRole != ROLES::TRAPPER) // trapper renders regular primary as secondary
				if (animPrimary == false)
					anim_first_current = AnimationState::first_secondary_idle;

			// *** third person defaults ***
			anim_third_current = AnimationState::third_primary_idle;
			//only brute and manipulator has a secondary 3rd person render
			if (animRole == ROLES::BRUTE || animRole == ROLES::MANIPULATOR)
				if (animPrimary == false)
					anim_third_current = AnimationState::third_secondary_idle;

			//instant stop
			if (grounded && isLocalPlayer)
			{
				airVelocity = vel = vec3(0);
			}

			if (!lockControls)
				if (i->getKeyInfo(controls.dance))
					animOverideIfPriority(anim_third_current, AnimationState::dance);

			if (!lockControls && !dancing)
			{
				//dance key

				//If freecam or spectating dont take player move input
				if (freecam == false && isDead == false)
				{
					roomID = cam->roomID;

					dir = cam->getDir();
					vec2 tempvec = vec2(0, 0);

					if (!grounded)//IN THE AIR YO
					{
						/*
						Att lägga till: Kolla så att hastigheten I DEN GIVNA RIKTNINGEN
						är mindre än tillåtet innan speed läggs till.
						Detta på alla knappar, inte bara W och S.
						*/
						if (i->getKeyInfo(controls.forward))
						{
							vel += normalize(glm::vec3(dir.x, 0, dir.z))*dt*0.3f;
						}

						if (i->getKeyInfo(controls.back))
						{
							vel -= normalize(glm::vec3(dir.x, 0, dir.z))*dt*0.3f;
						}

						if (!(i->getKeyInfo(controls.left) && i->getKeyInfo(controls.right)))
						{
							if (i->getKeyInfo(controls.left))
							{
								vec3 left = cross(vec3(0, 1, 0), dir);
								if (length(left) > 0)
									vel += normalize(left)*dt*0.4f;
							}
							if (i->getKeyInfo(controls.right))
							{
								vec3 right = cross(dir, vec3(0, 1, 0));
								if (length(right) > 0)
									vel += normalize(right)*dt*0.4f;
							}
						}

						
						glm::vec2 xzspeed = glm::vec2(vel.x, vel.z); //For calculating maximum allowed speed
						if (length(xzspeed) > maxVelocityXZ)
						{
							xzspeed = normalize(xzspeed)*maxVelocityXZ;
							vel.x = xzspeed.x;
							vel.z = xzspeed.y;
						}

						//Y speed is separate
						if (vel.y > maxVelocityY)
							vel.y = maxVelocityY;
					}
					else if (grounded)
					{
						if (i->getKeyInfo(controls.forward))
						{
							tempvec += normalize(vec2(dir.x, dir.z));
						}

						if (i->getKeyInfo(controls.back))
						{
							tempvec += -normalize(vec2(dir.x, dir.z));
						}

						if (!(i->getKeyInfo(controls.left) && i->getKeyInfo(controls.right)))
						{
							if (i->getKeyInfo(controls.left))
							{
								vec3 left = cross(vec3(0, 1, 0), dir);
								tempvec += normalize(vec2(left.x, left.z));
							}
							if (i->getKeyInfo(controls.right))
							{
								vec3 right = cross(dir, vec3(0, 1, 0));
								tempvec += normalize(vec2(right.x, right.z));
							}
						}

						if (length(tempvec) > 0)
						{
							tempvec = normalize(tempvec);
							vel.x = tempvec.x;
							vel.z = tempvec.y;
						}
					}


					Special* mobility = role.getMobilityAbility();
					bool mobileActivate = false;
					if (mobility->isThisOnJumpKey())
					{
						if (i->justPressed(controls.jump))
							mobileActivate = true;
					}
					else
					{
						if (i->justPressed(controls.mobility))
							mobileActivate = true;
					}
					if (mobileActivate)
					{
						if (mobility->allowedToActivate(this))
						{
							if (mobility->getActivationCost() < role.getSpecialMeter())
							{

								role.setSpecialMeter(role.getSpecialMeter() - mobility->getActivationCost());
								msg = MOBILITYUSE;
							}
						}
					}

					if (i->getKeyInfo(controls.jump))
					{
						if (grounded)
						{
							vel.y = role.getJumpHeight() * 5;
							airVelocity = vel;
							if (GetSoundActivated())
							{
								if (isLocal() || spectatingThisPlayer)
								{
									GetSound()->PlayStereoJump(role.getRole());
								}
								else
								{
									GetSound()->playJump(role.getRole(), pos.x, pos.y, pos.z);
								}
							}	
						}
					}

					if (!role.getIfBusy())
					{
						if (i->justPressed(controls.reload))
						{
							reloadCurrentWeapon();
						}
						int areWeScrolling = i->getScrollValue();

						if (areWeScrolling < 0.0 || i->justPressed(controls.weaponone))
						{
							if (role.getWeaponNRequiped() != 0)
							{
								animOverideIfPriority(anim_first_current, AnimationState::first_secondary_switch);
								if (animRole == ROLES::BRUTE)
								{
									animOverideIfPriority(anim_third_current, AnimationState::third_secondary_switch);
									animOverideIfPriority(anim_first_current, AnimationState::first_primary_switch_IN);
								}
								else if (animRole == ROLES::MANIPULATOR)
								{
									animOverideIfPriority(anim_third_current, AnimationState::third_secondary_switch);
									if (role.getCurrentWeapon()->getType() == WEAPON_TYPE::BATTERYWPN_SLOW)
										animOverideIfPriority(anim_first_current, AnimationState::first_secondary_switch_IN);
								}

								role.swapWeaponLocal(0);
								msg = WPNSWITCH;	

								animSwapActive = true;
								areWeScrolling = 0.0;
							}
						}

						if (areWeScrolling > 0.0 || i->justPressed(controls.weapontwo))
						{
							if (role.getWeaponNRequiped() != 1)
							{
								msg = WPNSWITCH;

								if (animRole == ROLES::TRAPPER)
								{
									if (role.getCurrentWeapon()->getCurrentAmmo() > 1)
									{
										role.swapWeapon(WEAPON_TYPE::ENERGY_BOOST, 1);
										animPrimary = false;
										msg = SHOOT;
										shoot();
										animPrimary = true;
									}
									else
									{
										reloadCurrentWeapon();
										msg = NONE;
									}
								}
								else
								{
									role.swapWeaponLocal(1);
									animOverideIfPriority(anim_first_current, AnimationState::first_primary_switch);
								}

								if (animRole == ROLES::BRUTE)
								{
									animOverideIfPriority(anim_first_current, AnimationState::first_secondary_switch_IN);
									animOverideIfPriority(anim_third_current, AnimationState::third_primary_switch);
								}
								else if (animRole == ROLES::MANIPULATOR)
								{
									animOverideIfPriority(anim_third_current, AnimationState::third_primary_switch);
									if (role.getCurrentWeapon()->getType() == WEAPON_TYPE::BATTERYWPN_SLOW)
										animOverideIfPriority(anim_first_current, AnimationState::first_primary_switch_IN);
								}

								animSwapActive = true;

							}
						}

						if (i->getKeyInfo(controls.fire))		//Temp
						{
							if (role.getCurrentWeapon()->shoot())
							{
								msg = SHOOT;
								shoot();
							}
							else if (role.getCurrentWeapon()->getCurrentAmmo() == 0)
							{
								reloadCurrentWeapon();
							}
						}

						if (i->justPressed(controls.item))
						{

							Consumable* c = role.getConsumable();
							if (c->use())
							{
								msg = USEITEM;
								animOverideIfPriority(anim_first_current, AnimationState::first_primary_throw);
							}

						}
					}
					if (i->justPressed(controls.special))
					{
						if (role.getSpecialAbility()->allowedToActivate(this))
						{
							msg = SPECIALUSE;
						}
					}
				} // end of player input
			} // end of lock control check

			if (role.getHealth() <= 0 && !isDead && role.getRole() != ROLES::NROFROLES)
			{
				isDead = true;
				msg = DEATH;
				respawnTimer = respawnTime;
				vel = glm::vec3(0, 0, 0);

				if (GetSoundActivated())
				{
					GetSound()->playUserGeneratedSound(SOUNDS::soundEffectYouDied, CATEGORY::Effects);
				}
			}

			if (isDead && respawnTimer != 0.0f)
			{
				respawnTimer -= dt;
				if (respawnTimer < FLT_EPSILON)
				{
					respawnTimer = 0.0f;
					msg = PLAYERRESPAWN;
					if (GetSoundActivated())
					{
						GetSound()->playUserGeneratedSound(SOUNDS::soundEffectRespawn, CATEGORY::Effects);
					}
				}
			}

			movementAnimationChecks(dt);

			modifiersSetData(dt);	//Dont Remove Again Please!
			movePlayer(dt, olddir); //This moves the player regardless of what we might end up colliding with


			clearCollisionNormals(); //Doesn't actually clear the array, just manually sets size to 0. This is to speed things up a little.
			clearEffectCollNormals();
		} // end of local player check
		else
		{
			/*
			THIS IS NOT THE LOCAL PLAYER
			*/
			goalTimer += dt;
			float t = goalTimer / interpolationTick;

			if (t > 1.0f)
				t = 1.0f;

			pos = (oldPos * (1.0f - t)) + (goalpos * t);
			glm::vec3 prev = dir;
			dir = (oldDir * (1.0f - t)) + (goaldir * t);

			rotatePlayer(prev, dir);

			if (spectatingThisPlayer == true)
			{
				cam->roomID = roomID;

				cam->setCam(pos, dir);

				animRole = role.getRole();
				animPrimary = false;
				if (role.getWeaponNRequiped() == 0) // if primary
					animPrimary = true;

				if (anim_first_framePeak == AnimationState::first_primary_reload ||
					anim_first_framePeak == AnimationState::first_secondary_reload)
					specReoloadSound++;
				else
					specReoloadSound = 0;

				if (specReoloadSound == 1)
					reloadCurrentWeapon(true);

				//special case when spectating a brute and he swaps weapons

				if (animRole == ROLES::BRUTE && animSwapActive == false)
				{
					//init swap 1st to second
					if (anim_first_framePeak == AnimationState::first_primary_switch)
					{
						animSwapTime_OUT = 0.53f * 0.5f;
						animSwapActive = true;
					}
					//init swap 2nd to first
					else if (anim_first_framePeak == AnimationState::first_secondary_switch)
					{
						animSwapTime_OUT = 0.34f * 0.5f;
						animSwapActive = true;
					}
				}

				if (animSwapActive && animSwapTime_OUT > 0)
				{
					animPrimary = !animPrimary;
					animSwapTime_OUT -= dt;
					if (animSwapTime_OUT < 0.001f) // out swap timeout
					{
						if (animPrimary)
							animOverideIfPriority(anim_first_current, AnimationState::first_secondary_switch_IN);
						else
							animOverideIfPriority(anim_first_current, AnimationState::first_primary_switch_IN);

						animPrimary = !animPrimary;
						animSwapActive = false;
					}
				} // spec brute animswap end
			} // spectating this end

			if (role.getHealth() == 0)
			{
				isDead = true;
				vel = glm::vec3(0, 0, 0);
				cleanseModifiers();
			}
			modifiersSetData(dt);
		}
	}
	else //We have yet to pick a class, so stop fucking moving
	{
		pos.x = 9999.0f;
		pos.y = 9999.0f;
		pos.z = 9999.0f;
	}

	return msg;
}

void Player::movementUpdates(float dt, bool freecam, bool spectatingThisPlayer, bool spectating)
{
	/*
	This update loop has information about collisions, and as such will place the player in a legal position while maintaining frame-perfect collision data
	aka we are no longer 1 frame desynced

	In this update, we also take care of external forces such as explosions and gravity
	*/

	vec3 olddir = cam->getDir();
	if (isLocalPlayer)
	{
		//ignore if we are spectating
		if (currentTeam != 0)
		{
			movePlayerCollided(dt, olddir);

			if (!freecam && !spectating)
				cam->setCam(pos);

			float lastHeight = pos.y;

			if (freecam && spectating == false)
			{
				if (GetSoundActivated())
				{
					GetSound()->setLocalPlayerPos(cam->getPos());
					GetSound()->setLocalPlayerDir(cam->getDir());
				}
			}
		}
	}

	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y; //head offset. player objects have their origo at their feet
	worldMat[2].w = pos.z;

	if (freecam == true && spectatingThisPlayer == false)
	{
		worldMat[1].w -= 1.45f;  // move down if 3rd person render
	}

	if (freecam == false && isLocalPlayer == false && spectatingThisPlayer == false)
	{
		worldMat[1].w -= 1.45f; // move down if 3rd person render
	}
}

int Player::getAmmo()
{
	if (role.getRole() != ROLES::NROFROLES)
	{
		return role.getAmmo();
	}
	return 0;
}

int Player::getMaxAmmo()
{
	if (role.getRole() != ROLES::NROFROLES)
	{
		return role.getMaxAmmo();
	}
	return 0;
}

void Player::rotatePlayer(vec3 olddir, vec3 newdir)
{
	float angle = atan2(newdir.x, newdir.z) - atan2(olddir.x, olddir.z);
	rotate(0, -angle, 0);
	dir = newdir;
}

void Player::reloadCurrentWeapon(bool force)
{
	if (!role.getCurrentWeapon()->getIfFullAmmo() || force)
	{
		if (GetSound())
		{
			if (animRole == 0)
				GetSound()->playUserGeneratedSound(SOUNDS::soundEffectTrapperReload, CATEGORY::Guns);
			else if (animRole == 2)
				GetSound()->playUserGeneratedSound(SOUNDS::soundEffectStalkerReload, CATEGORY::Guns);
			else if (animRole == 3)
				GetSound()->playUserGeneratedSound(SOUNDS::soundEffectPunisherReload, CATEGORY::Guns);

		}

		role.getCurrentWeapon()->reload();

		if (animRole == ROLES::BRUTE)
		{
			if (animPrimary)
				animOverideIfPriority(anim_first_current, AnimationState::first_primary_reload);
			else
				animOverideIfPriority(anim_first_current, AnimationState::first_secondary_reload);
		}
		else
			animOverideIfPriority(anim_first_current, AnimationState::first_primary_reload);
	}
}

Weapon* Player::getPlayerCurrentWeapon()
{
	return role.getCurrentWeapon();
}

void Player::switchWpn(WEAPON_TYPE ws, int swapLoc)
{
	role.swapWeapon(ws, swapLoc);
}

void Player::shoot()
{
	if (animPrimary) //main weapon
		animOverideIfPriority(anim_first_current, AnimationState::first_primary_fire);
	else // secondary fire
	{
		animOverideIfPriority(anim_first_current, AnimationState::first_secondary_fire);

		if (animRole == ROLES::MOBILITY)
			animOverideIfPriority(anim_third_current, AnimationState::third_shankbot_melee_standing);

		if (animRole == ROLES::MANIPULATOR)
		{
			if (role.getCurrentWeapon()->getType() == WEAPON_TYPE::BATTERYWPN_SLOW)
				animOverideIfPriority(anim_first_current, AnimationState::first_secondary_fire);

			else
				animOverideIfPriority(anim_first_current, AnimationState::first_secondary_fire_left);
				
		}
	}

	//Add a bullet recoil factor that is multiplied by a random number and smooth it out
}

void Player::hitByBullet(Bullet* b, BULLET_TYPE bt, int newHPtotal)
{
	/*
	This is where we will extract additional modifiers from the Bullet, when applicable.
	*/
	if (newHPtotal == -1) //We are actually taking damage on the server now
	{
		if (!isDead)
		{
			if (b != nullptr)
			{
				int dmg = b->getDamage();
				role.takeDamage(dmg);
			}
			else
				role.takeDamage(20);
		}
	}
	else //We are on a client, and thus are only interested on our HP on the server
	{
		if (!isDead)
			role.setHealth(newHPtotal);
	}

	if (bt == BULLET_TYPE::HACKING_DART)
	{
		addModifier(MODIFIER_TYPE::HACKINGDARTMODIFIER);
	}
}

void Player::hitByEffect(Effect* e, EFFECT_TYPE et, int newHPtotal)
{
	/*
	Big ol' switch case to identify which effect is hitting us and what we should do about it
	*/

	if (newHPtotal == -1) //This is the server, dealing damage to the player
	{
		if (e != nullptr)
		{
			int dmg = e->getDamage();
			role.takeDamage(dmg);
		}
		else
		{
			if (et == EFFECT_TYPE::EXPLOSION)
				role.takeDamage(20); //TEMPORARY SOLUTION, MUST FIND ORIGIN OF PROBLEM
		}
			
	}
	else //Hello I'm the client. I accept my new HP.
	{
		role.setHealth(newHPtotal);
	}
}

void Player::applyGravity(float vel)
{
	if (!grounded)
		this->vel.y -= vel;
}

void Player::addModifier(MODIFIER_TYPE mt)
{
	Modifier* m = nullptr;
	switch (mt)
	{
	case LIGHTWALLCONTROLLOCK:
	{
		m = new LightWallLockedControls();
	}
	break;
	case MODIFIER_TYPE::HACKINGDARTMODIFIER:
	{
		m = new HackingDartModifier();
	}
	break;
	case MODIFIER_TYPE::LIGHTSPEEDMODIFIER:
	{
		m = new LightSpeed();
	}
	break;
	case MODIFIER_TYPE::OVERCHARGEMODIFIER:
	{
		m = new Overcharge();
	}
	break;
	case MODIFIER_TYPE::SPRINTCONTROLLOCK:
	{
		m = new SprintControlLock;
	}
	break;
	case MODIFIER_TYPE::TRUEGRITMODIFIER:
	{
		m = new TrueGrit();
	}
	break;
	case MODIFIER_TYPE::TRAPPERSHAREAMMO:
	{
		m = new TrapperShareAmmo();
	}
	break;
	case MODIFIER_TYPE::BATTERYSLOWMOD:
	{
		m = new BatterySlowMod();
	}
	break;
	case MODIFIER_TYPE::BATTERYSPEEDMOD:
	{
		m = new BatterySpeedMod();
	}
	break;
	case MODIFIER_TYPE::DOUBLEDAMAGEMOD:
	{
		m = new DoubleDamageMod();
	}
	break;
	}
	m->init(this);
	myModifiers.push_back(m);
}

void Player::setRole(Role role)
{
	this->role = role;
}

void Player::respawn(glm::vec3 respawnPos, glm::vec3 _dir, int _roomID)
{
	//reset matrix
	worldMat = mat4();
	rotatePlayer(vec3(0, 0, 1), _dir);
	goalpos = pos = respawnPos;

	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y;
	worldMat[2].w = pos.z;
	goaldir = _dir;

	vel = glm::vec3(0, 0, 0);
	if (isLocalPlayer)
		cam->setCam(pos, dir);
	isDead = false;
	cleanseModifiers();
	role.returnToLife();

	roomID = _roomID;

	if (isLocalPlayer)
		cam->roomID = _roomID;
}

void Player::healing(int amount)
{
	role.setHealth(role.getHealth() + amount);
}

bool Player::searchModifier(MODIFIER_TYPE search)
{
	for (int i = 0; i < myModifiers.size(); i++)
		if (myModifiers[i]->getType() == search)
			return true;

	return false;
}

Modifier* Player::searchModifierGet(MODIFIER_TYPE search)
{
	for (int i = 0; i < myModifiers.size(); i++)
		if (myModifiers[i]->getType() == search)
			return myModifiers[i];

	return nullptr;
}

glm::mat4 Player::getFPSmat()
{
	mat4 ret = glm::lookAt(cam->getPos(), cam->getPos() + cam->getDir() * -1.0f, vec3(0, 1, 0));

	vec3 tangentRight = cross(cam->getDir(), vec3(0, 1, 0));
	vec3 tangentDown = -cross(tangentRight, cam->getDir());

	float yOffset = 0.0f;
	float xOffset = 0.0f;
	float zOffset = 0.0f;

	switch (role.getRole())
	{
	case TRAPPER:
		yOffset = 0.0f;
		xOffset = 0.0f;
		zOffset = -0.0f;
		break;
	case DESTROYER:
		yOffset = 0.3f;
		xOffset = 0.0f;
		zOffset = 0.2f;
		break;
	case MOBILITY:
		yOffset = -0.10f;
		xOffset = -0.05f;
		zOffset = -0.25f;
		break;
	case BRUTE:
		if (animPrimary)
		{
			yOffset = -0.14f;
			xOffset = -0.47f;
			zOffset = 0.12f;
		}
		else
		{
			yOffset = 0.32f;
			xOffset = 0.45f;
			zOffset = 0.28f;
		}
		break;
	case MANIPULATOR:
		yOffset = 0.0f;
		xOffset = 0.0f;
		zOffset = 0.0f;
		break;
	}
	vec3 renderpos = cam->getDir() * zOffset + normalize(tangentDown) * yOffset + normalize(tangentRight) * xOffset + cam->getPos();

	ret[0].w += renderpos.x;
	ret[1].w += renderpos.y;
	ret[2].w += renderpos.z;

	return ret;
}

void Player::deadViewAngles()
{
	//if dead lower cam test
	if (getTeam() != 0)
	{
		if (isDead)
		{
			switch (animRole)
			{
			case TRAPPER:		cam->setCam(cam->getPos() + vec3(0, -0.7f, 0));			break;
			case DESTROYER:		cam->setCam(cam->getPos() + vec3(0, -1, 0));			break;
			case MOBILITY:		cam->setCam(cam->getPos() + vec3(0, -1, 0));			break;
			case BRUTE:			cam->setCam(cam->getPos() + vec3(0, -0.7f, 0));			break;
			case MANIPULATOR:	if(animPrimary || deathTimer > 0.8f)	cam->setCam(cam->getPos() + vec3(0, -1, 0));break;
			default:																	break;
			}
		}
		else
		{

		}
	}
}

void Player::movmentSpecialAnimUse(int react)
{
	switch (animRole)
	{
	case TRAPPER:		animOverideIfPriority(anim_third_current, AnimationState::third_primary_jump_begin);	return;
	case DESTROYER:		return;
	case MOBILITY:
	{
		switch (react)
		{
		case 0: animOverideIfPriority(anim_third_current, AnimationState::third_primary_jump_begin);			return;
		case 1:	animOverideIfPriority(anim_third_current, AnimationState::third_shankbot_walljump_right);		return;
		case 2:	animOverideIfPriority(anim_third_current, AnimationState::third_shankbot_walljump_left);		return;
		}

	}
	case BRUTE:			return;
	case MANIPULATOR:	return;
	}
}

bool Player::getAnimPrimary()
{
	if (animRole == ROLES::BRUTE)
	{
		return animPrimary;
	}
	return true;
}

void Player::movementAnimationChecks(float dt)
{
	//manipulator checks
	bool manipSecondLeft = false;
	
	if (animRole == ROLES::MANIPULATOR)
		if (animPrimary == false)
			if (role.getCurrentWeapon()->getType() == WEAPON_TYPE::BATTERYWPN_SLOW)
			{
				manipSecondLeft = true;
				animOverideIfPriority(anim_first_current, AnimationState::first_secondary_idle_left);
			}

	if (grounded)
	{
		//Run checks
		if (vel.x != 0 || vel.z != 0)
		{
			if (animPrimary == true || (animRole != ROLES::MANIPULATOR && animRole != ROLES::BRUTE))
			{
				//first person run
				animOverideIfPriority(anim_first_current, AnimationState::first_primary_run);
				if (animPrimary == false)
					if (animRole != ROLES::TRAPPER) // trapper has no secondary weapon anims
						animOverideIfPriority(anim_first_current, AnimationState::first_secondary_run);

				//Third person

				// strafe left
				if (i->getKeyInfo(controls.left) && !i->getKeyInfo(controls.forward) && !i->getKeyInfo(controls.back))
					animOverideIfPriority(anim_third_current, AnimationState::third_primary_strafe_left);

				// strafe right
				else if (i->getKeyInfo(controls.right) && !i->getKeyInfo(controls.forward) && !i->getKeyInfo(controls.back))
					animOverideIfPriority(anim_third_current, AnimationState::third_primary_strafe_right);

				//check if we are running backwards
				else if (dot(vel, dir) < 0)
					animOverideIfPriority(anim_third_current, AnimationState::third_primary_run_rev);

				else //run forward
				{
					animOverideIfPriority(anim_third_current, AnimationState::third_primary_run);
					if (role.getRole() == ROLES::MOBILITY)
						if (searchModifier(MODIFIER_TYPE::LIGHTSPEEDMODIFIER))
							animOverideIfPriority(anim_third_current, AnimationState::third_shankbot_charge);
				}
			}
			else // brute and manipulator secondary weapon case
			{
				animOverideIfPriority(anim_first_current, AnimationState::first_secondary_run);

				if(manipSecondLeft)
					animOverideIfPriority(anim_first_current, AnimationState::first_secondary_run_left);

				//Third person

				// strafe left
				if (i->getKeyInfo(controls.left) && !i->getKeyInfo(controls.forward) && !i->getKeyInfo(controls.back))
					animOverideIfPriority(anim_third_current, AnimationState::third_secondary_strafe_left);

				// strafe right
				else if (i->getKeyInfo(controls.right) && !i->getKeyInfo(controls.forward) && !i->getKeyInfo(controls.back))
					animOverideIfPriority(anim_third_current, AnimationState::third_secondary_strafe_right);

				//check if we are running backwards
				else if (dot(vel, dir) < 0)
					animOverideIfPriority(anim_third_current, AnimationState::third_secondary_run_rev);

				else //run forward
					animOverideIfPriority(anim_third_current, AnimationState::third_secondary_run);
			}
		}
	}
	else //if in air
	{
		animOverideIfPriority(anim_third_current, AnimationState::third_primary_air);
		if (animPrimary == false)
			if (animRole == ROLES::MANIPULATOR || animRole == ROLES::BRUTE)
				animOverideIfPriority(anim_third_current, AnimationState::third_secondary_air);
	}

	//Jump Checks
	if (grounded != animGroundedLast) //grounded changed this frame
	{
		if (grounded) //landed
		{
			animOverideIfPriority(anim_third_current, AnimationState::third_primary_jump_end);
			if (animPrimary == false)
				if (animRole == ROLES::MANIPULATOR || animRole == ROLES::BRUTE)
					animOverideIfPriority(anim_third_current, AnimationState::third_secondary_jump_end);

			if (GetSoundActivated())
			{
				GetSound()->playLand(getRole()->getRole(), pos.x, pos.y, pos.z);
			}

		}
		else // jump begin
		{
			animOverideIfPriority(anim_third_current, AnimationState::third_primary_jump_begin);
			if (animPrimary == false)
				if (animRole == ROLES::MANIPULATOR || animRole == ROLES::BRUTE)
					animOverideIfPriority(anim_third_current, AnimationState::third_secondary_jump_begin);
		}
	}
	animGroundedLast = grounded;

	//specal case movement
	if (animRole == ROLES::BRUTE)
	{
		Modifier* searchmod = searchModifierGet(MODIFIER_TYPE::TRUEGRITMODIFIER);
		if (searchmod != nullptr)
			if (!((TrueGrit*)searchmod)->isDashDone())
			{
				if (animPrimary)
					animOverideIfPriority(anim_third_current, AnimationState::third_brute_primary_dash);
				else
					animOverideIfPriority(anim_third_current, AnimationState::third_brute_secondary_dash);
			}
	}

	//weapon swap special case on brute because of 2 weapon meshes and special animations
	if (animSwapActive && animRole == ROLES::BRUTE)
	{
		//init swap timers
		if (lastanimSwapActive == false)
		{
			if (animPrimary)
				animSwapTime_OUT = 0.53f * 0.5f;
			else
				animSwapTime_OUT = 0.34f * 0.5f;

			if (animPrimary == false)
				animOverideIfPriority(anim_first_current, AnimationState::first_secondary_switch);
			else
				animOverideIfPriority(anim_first_current, AnimationState::first_primary_switch);
		}
		else
		{
			if (animSwapTime_OUT > 0)
			{
				animPrimary = !animPrimary;
				animSwapTime_OUT -= dt;
				if (animSwapTime_OUT < 0.001f) // out swap timeout
				{
					if (animPrimary)
						animOverideIfPriority(anim_first_current, AnimationState::first_secondary_switch_IN);
					else
						animOverideIfPriority(anim_first_current, AnimationState::first_primary_switch_IN);

					animPrimary = !animPrimary;
					animSwapActive = false;
				}
			}
		}
		lastanimSwapActive = animSwapActive;
	}

	//death checks
	if (isDead != animLastDead)
	{
		if (isDead)
		{
			animOverideIfPriority(anim_third_current, AnimationState::third_primary_death);
			if(animPrimary)
				animOverideIfPriority(anim_first_current, AnimationState::first_primary_death);
			else
			{
				animOverideIfPriority(anim_first_current, AnimationState::first_secondary_death);
				if(manipSecondLeft)
					animOverideIfPriority(anim_first_current, AnimationState::first_secondary_death_left);
			}
		}
		else
		{
			anim_third_current = AnimationState::none;
			anim_first_current = AnimationState::none;
			anim_third_framePeak = AnimationState::none;
			anim_first_framePeak = AnimationState::none;
		}
	}
	animLastDead = isDead;

	animOverideIfPriority(anim_third_framePeak, anim_third_current);
	animOverideIfPriority(anim_first_framePeak, anim_first_current);
}

void Player::chooseRole(int r)
{
	cleanseModifiers(true);
	role.chooseRole(r);
	if (r == ROLES::TRAPPER)
	{
		addModifier(MODIFIER_TYPE::TRAPPERSHAREAMMO);
	}
}

void Player::IncreaseFrags()
{
	this->consecutiveFrags++;
}

void Player::IncreaseDeaths()
{
	this->consecutiveDeaths++;
}


void Player::ZeroFrags()
{
	this->consecutiveFrags = 0;
	this->killingSpreeDone = false;
	this->impressiveDone = false;
}

void Player::ZeroDeaths()
{
	this->consecutiveDeaths = 0;
	roleChangeWritten = false;
}

int Player::GetConsecutiveFrags()
{
	return this->consecutiveFrags;
}

void Player::SetJumpCoolDown(float cooldown)
{
	this->jumpSoundCooldown = cooldown;
	jumpSoundAble = false;
}

void Player::CoolDownJump(float dt)
{
	if (this->jumpSoundCooldown > FLT_EPSILON)
	{
		this->jumpSoundCooldown -= dt;
	}
	
	else jumpSoundAble = true;
}

bool Player::CheckAbleToJumpSound()
{
	return jumpSoundAble;
}

bool Player::allahuAkhbar()
{
	if (pos.y < -5.0f && !isDead && role.getRole() != NROFROLES)	//If we fall through the map we die
	{
		return true;
	}
	return false;
}

void Player::fixCamera(float dt, bool freecam, bool spectating)
{
		if (!freecam && !spectating)
		{
			cam->update(dt, freecam);
			if (!isDead)
			{
				cam->setCam(pos);
				if (!dancing)
					rotatePlayer(oldDir, dir);
				else
					cam->setCam(pos, dir);
			}
			else
				cam->setCam(pos, dir);
		}
		else if (!spectating)
		{
			cam->update(dt, freecam);
		}
}