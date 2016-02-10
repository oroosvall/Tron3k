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
	dir = cam->getDir();

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

void Player::movePlayer(float dt, glm::vec3 oldDir, bool freecam, bool spectating)
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
		GetSound()->setLocalPlayerPos(pos);
		GetSound()->setLocalPlayerDir(cam->getDir());
	}

	if ((vel.x != 0 || vel.z != 0) && this->grounded)
	{

		if (this->getFootsteps() && this->getGrounded() && GetSoundActivated() && this->role.getRole() != 1)
		{
			this->setFootstepsCountdown();
			this->setFootstepsLoop(false);
			GetSound()->playFootsteps(this->role.getRole(), pos.x, pos.y, pos.z);
		}

		if (this->role.getRole() == 1 && GetSoundActivated() && GetSound()->destroyerPaused == true)
		{

			GetSound()->playFootsteps(this->role.getRole(), pos.x, pos.y, pos.z);
			GetSound()->destroyerPaused = false;
		}

		/*	else if (this->role.getRole() == 3 && GetSoundActivated() && GetSound()->brutePaused == true)
		{
		GetSound()->playFootsteps(this->role.getRole(), pos.x, pos.y, pos.z);
		GetSound()->brutePaused = false;
		}*/
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

void Player::movePlayerCollided(float dt, glm::vec3 oldDir, bool freecam, bool specingThis)
{
	//get normals
	//std::vector<vec4> cNorms = physics->sphereVWorldCollision(pos - (vec3(0, 0.55f, 0)), 1);
	effectCollisionHandling();
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

			// abslut value, if two collisions from the same angle they should not move us twice the distance
			//if (length(pendepth) > length(posadjust))
				//posadjust = pendepth;
			
			if (abs(pendepth.x) > abs(posadjust.x))
				posadjust.x += pendepth.x;
			if (abs(pendepth.y) > abs(posadjust.y))
				posadjust.y += pendepth.y;
			if (abs(pendepth.z) > abs(posadjust.z))
				posadjust.z += pendepth.z;

			/*if (pendepth.x + FLT_EPSILON > 0.0f || pendepth.x - FLT_EPSILON < 0.0f)//abs(posadjust.x) < abs(pendepth.x))
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
		posadjust = posadjust;
		

		if (ceiling)
		{
			posadjust.y = 0;
			grounded = false;
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
		posadjust = posadjust;


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
	else
	{
		collided = false;
		airVelocity = vel;
		grounded = false;
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
		this->footstepsCountdown = 0.29;
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



	if (grounded)
		int x = 0;
	else
		int x = 1;

	if (collided)
		int x = 0;
	else
		int x = 1;

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

		if (i->justPressed(GLFW_KEY_ENTER))
		{
			if (lockControls)
				lockControls = false;
			else
				lockControls = true;
		}

		if (noclip)
			vel *= 0;

		//friction
		//if (grounded)
		//{
		//	vel -= vec3(vel.x, 0, vel.z) * dt * 30.0f;
		//}

		//instant stop
		if (grounded && isLocalPlayer)
		{
			airVelocity = vel = vec3(0);
		}

		if (!lockControls)
		{
			//move camera to where we are looking.
			//if freecam is true the cam can move on its own
			if (spectating == false)
			{
				if (i->justPressed(GLFW_KEY_C)) // flymode
					noclip = !noclip;

				if (noclip)
				{
					cam->update(dt, true);
					setPos(cam->getPos());
					vel *= 0;
				}
				else
					cam->update(dt, freecam);
			}

			//If freecam or spectating dont take player move input
			if (freecam == false)
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
					if (i->getKeyInfo(GLFW_KEY_W))
					{
						//if (length(glm::vec2(airVelocity.x, airVelocity.z)) < role.getMovementSpeed()*0.1f)
						//{
						vel += normalize(glm::vec3(dir.x, 0, dir.z))*dt*0.3f;
						//}
					}

					if (i->getKeyInfo(GLFW_KEY_S))
					{
						//if (length(glm::vec2(airVelocity.x, airVelocity.z)) < role.getMovementSpeed()*0.1f)
						//{
						vel -= normalize(glm::vec3(dir.x, 0, dir.z))*dt*0.3f;
						//}
					}

					if (!(i->getKeyInfo(GLFW_KEY_A) && i->getKeyInfo(GLFW_KEY_D)))
					{
						if (i->getKeyInfo(GLFW_KEY_A))
						{
							vec3 left = cross(vec3(0, 1, 0), dir);
							if (length(left) > 0)
								vel += normalize(left)*dt*0.4f;
						}
						if (i->getKeyInfo(GLFW_KEY_D))
						{
							vec3 right = cross(dir, vec3(0, 1, 0));
							if (length(right) > 0)
								vel += normalize(right)*dt*0.4f;
						}
					}
				}
				else if (grounded)
				{
					if (i->getKeyInfo(GLFW_KEY_W))
					{
						tempvec += normalize(vec2(dir.x, dir.z));
					}

					if (i->getKeyInfo(GLFW_KEY_S))
					{
						tempvec += -normalize(vec2(dir.x, dir.z));
					}

					if (!(i->getKeyInfo(GLFW_KEY_A) && i->getKeyInfo(GLFW_KEY_D)))
					{
						if (i->getKeyInfo(GLFW_KEY_A))
						{
							vec3 left = cross(vec3(0, 1, 0), dir);
							tempvec += normalize(vec2(left.x, left.z));
						}
						if (i->getKeyInfo(GLFW_KEY_D))
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
				if (i->justPressed(mobility->getActivationKey()))
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

				if (i->getKeyInfo(GLFW_KEY_SPACE))
				{
					if (grounded)
					{
						vel.y = role.getJumpHeight() * 5;
						airVelocity = vel;
						if (GetSoundActivated())
							GetSound()->playJump(role.getRole(), pos.x, pos.y, pos.z);
					}
				}

				if (!role.getIfBusy())
				{
					if (i->justPressed(GLFW_KEY_R))
					{
						reloadCurrentWeapon();
					}

					if (i->justPressed(GLFW_KEY_1))
					{
						if (role.getWeaponNRequiped() != 0)
						{
							role.swapWeaponLocal(0);
							msg = WPNSWITCH;

							animOverideIfPriority(anim_first_current, AnimationState::first_secondary_switch);
							if (animRole == ROLES::BRUTE)
							{
								animOverideIfPriority(anim_third_current, AnimationState::third_secondary_switch);
								animOverideIfPriority(anim_first_current, AnimationState::first_primary_switch_IN);
							}

							animSwapActive = true;
						}
					}

					if (i->justPressed(GLFW_KEY_2))
					{
						if (role.getWeaponNRequiped() != 1)
						{
							msg = WPNSWITCH;

							if (animRole == ROLES::TRAPPER)
							{
								if (role.getCurrentWeapon()->getCurrentAmmo() > 1)
								{
									role.swapWeaponLocal(1);
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

							animSwapActive = true;

						}
					}

					if (i->getKeyInfo(GLFW_MOUSE_BUTTON_LEFT))		//Temp
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

					if (i->justPressed(GLFW_KEY_Q))
					{

						Consumable* c = role.getConsumable();
						if (c->use())
						{
							msg = USEITEM;
							animOverideIfPriority(anim_first_current, AnimationState::first_primary_throw);
						}

					}
				}
				if (i->justPressed(GLFW_KEY_E))
				{
					/*
					Add logic (in role) that checks against the applicable special and other conditions
					*/
					if (role.getSpecialAbility()->allowedToActivate(this))
					{
						msg = SPECIALUSE;
					}
				}

				if (i->justPressed(GLFW_KEY_Z))					//Temp?
				{
					if (GetSoundActivated() == 0 && GetInitialized() == 0)
					{
						InitSound(CreateSound(), 1);
						GetSound()->playMusic(mainMenu);
					}
					else if (GetInitialized())
					{
						GetSound()->enableSounds();
					}
				}
				if (i->justPressed(GLFW_KEY_M))
					role.setSpecialMeter(100.0f);
			} // end of player input
		} // end of lock control check

		if (pos.y < -5.0f && isLocalPlayer)	//If we fall through the map we die
		{
			role.setHealth(0);
		}

		//if (i->justPressed(GLFW_KEY_O))
		//	role.setHealth(0);

		if (role.getHealth() <= 0 && !isDead && role.getRole() != ROLES::NROFROLES)
		{
			isDead = true;
			msg = DEATH;
			respawnTimer = respawnTime;
			vel = glm::vec3(0, 0, 0);

			if (GetSoundActivated())
			{
				GetSound()->playUserGeneratedSound(SOUNDS::soundEffectYouDied);
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
					GetSound()->playUserGeneratedSound(SOUNDS::soundEffectRespawn);
				}
			}
		}

		movementAnimationChecks(dt);

		modifiersSetData(dt);	//Dont Remove Again Please!
		movePlayer(dt, olddir, freecam, spectating); //This moves the player regardless of what we might end up colliding with


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

			animRole = role.getRole();
			animPrimary = false;
			if (role.getWeaponNRequiped() == 0) // if primary
				animPrimary = true;

			//special case when spectating a brute and he swaps weapons

			if (animRole == ROLES::BRUTE && animSwapActive == false)
			{
				//init swap 1st to second
				if (anim_first_framePeak == AnimationState::first_primary_switch)
				{
					animSwapTime_OUT = 0.53f;
					animSwapActive = true;
				}
				//init swap 2nd to first
				else if (anim_first_framePeak == AnimationState::first_secondary_switch)
				{
					animSwapTime_OUT = 0.34f;
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
			movePlayerCollided(dt, olddir, freecam, spectatingThisPlayer);
			if (!freecam || spectating == true)
			{
				cam->setCam(pos, dir);
				rotatePlayer(oldDir, dir);
			}

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

void Player::reloadCurrentWeapon()
{
	if (!role.getCurrentWeapon()->getIfFullAmmo())
	{
		if (GetSound())
		{
			if (this->role.getRole() == 0)
				GetSound()->playUserGeneratedSound(SOUNDS::soundEffectTrapperReload);
			else if (this->role.getRole() == 2)
				GetSound()->playUserGeneratedSound(SOUNDS::soundEffectStalkerReload);
			else if (this->role.getRole() == 3)
				GetSound()->playUserGeneratedSound(SOUNDS::soundEffectPunisherReload);

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

void Player::hitByEffect(Effect* e, int newHPtotal)
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
			role.takeDamage(20); //TEMPORARY SOLUTION, MUST FIND ORIGIN OF PROBLEM
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
		removeSpecificModifier(LIGHTWALLCONTROLLOCK);
		m = new LightWallLockedControls();
		myModifiers.push_back(m);
	}
	break;
	case MODIFIER_TYPE::HACKINGDARTMODIFIER:
	{
		m = new HackingDartModifier();
		myModifiers.push_back(m);
	}
	break;
	case MODIFIER_TYPE::LIGHTSPEEDMODIFIER:
	{
		m = new LightSpeed();
		myModifiers.push_back(m);
	}
	break;
	case MODIFIER_TYPE::OVERCHARGEMODIFIER:
	{
		m = new Overcharge();
		myModifiers.push_back(m);
	}
	break;
	case MODIFIER_TYPE::SPRINTCONTROLLOCK:
	{
		m = new SprintControlLock;
		myModifiers.push_back(m);
	}
	break;
	case MODIFIER_TYPE::TRUEGRITMODIFIER:
	{
		m = new TrueGrit();
		myModifiers.push_back(m);
	}
	break;
	case MODIFIER_TYPE::TRAPPERSHAREAMMO:
	{
		m = new TrapperShareAmmo();
		myModifiers.push_back(m);
	}
	break;
	}
	myModifiers[myModifiers.size() - 1]->init(this);
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
	pos = respawnPos;

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
	//printf("Now in room %d", _roomID);

	if (isLocalPlayer)
		cam->roomID = _roomID;

	justRespawned = true;
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
				if (i->getKeyInfo(GLFW_KEY_A) && !i->getKeyInfo(GLFW_KEY_W) && !i->getKeyInfo(GLFW_KEY_S))
					animOverideIfPriority(anim_third_current, AnimationState::third_primary_strafe_left);

				// strafe right
				else if (i->getKeyInfo(GLFW_KEY_D) && !i->getKeyInfo(GLFW_KEY_W) && !i->getKeyInfo(GLFW_KEY_S))
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

				//Third person

				// strafe left
				if (i->getKeyInfo(GLFW_KEY_A) && !i->getKeyInfo(GLFW_KEY_W) && !i->getKeyInfo(GLFW_KEY_S))
					animOverideIfPriority(anim_third_current, AnimationState::third_secondary_strafe_left);

				// strafe right
				else if (i->getKeyInfo(GLFW_KEY_D) && !i->getKeyInfo(GLFW_KEY_W) && !i->getKeyInfo(GLFW_KEY_S))
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
				animSwapTime_OUT = 0.53f;
			else
				animSwapTime_OUT = 0.34f;

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
			animOverideIfPriority(anim_third_current, AnimationState::third_primary_death);
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