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
	cam = CameraInput::getCam();
	i = Input::getInput();

	name = pName;
	pos = initPos;
	vel = glm::vec3(0, 0, 0);
	dir = cam->getDir();

	anim_first_current = AnimationState::first_primary_idle;
	anim_first_framePeak = AnimationState::first_primary_idle;
	anim_third_current = AnimationState::third_idle;
	anim_third_framePeak = AnimationState::third_idle;

	isLocalPlayer = isLocal;

	rotate(0, -3.141592654f, 0);
}

void Player::setName(std::string newName)
{
	name = newName;
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

void Player::movePlayer(float dt, glm::vec3 oldDir, bool freecam, bool specingThis)
{

	if (!this->getFootsteps())
	{
		this->footstepsLoopReset(dt);
	}

	if (vel.x != 0 || vel.z != 0)
	{

		if (this->getFootsteps() && this->getGrounded() && GetSoundActivated())
		{
			this->setFootstepsCountdown();
			this->setFootstepsLoop(false);
			GetSound()->playFootsteps(this->role.getRole(), pos.x, pos.y, pos.z);
		}

	}

	glm::vec3 playerVel = vec3(0);
	playerVel.x = vel.x*role.getMovementSpeed();
	playerVel.z = vel.z*role.getMovementSpeed();
	playerVel.y = vel.y;
	pos += playerVel * dt; //Here we will also include external forces.. EDIT: External forces moved, for now

	//Collision handling here, after movement
	bool ceiling = false;
	vec3 posadjust = vec3(0);
	if (collisionNormalSize > 0)
	{
		//grounded set to true at start of frame
		grounded = true;
		if (collisionNormalSize > 1)
			int x = 0;
		for (int k = 0; k < collisionNormalSize; k++)
		{
			//push pos away and lower velocity using pendepth
			vec3 pendepth = vec3(collisionNormals[k]) * collisionNormals[k].w;
			if (collisionNormals[k].y < 0)
				ceiling = true;

			if (collisionNormals[k].y > 0) // test ramp!!
			{
				//grounded = true;

			}
			else //If we don't collide to anything that is a floor, we don't touch ground
				grounded = false;
			posadjust += pendepth;

			//test code
			/*same effect as actual code
			pendepth = -pendepth;
			pendepth.y = 0;
			float projectionval = 0;
			if(dot(pendepth, pendepth) != 0)
			projectionval = dot(vel, pendepth) / dot(pendepth, pendepth);
			
			vel -= pendepth * projectionval;*/
		}

		playerVel += posadjust / dt * 0.5f;

		if (vel != vec3(0, 0, 0))//IF we don't move, we don't want to be moved
		{
			if (ceiling)
				posadjust.y = 0;

			//if(vel.x != 0 && vel.z != 0)
			pos += posadjust;
			if (posadjust != vec3(0, 0, 0))
			{
				//test equaitons
				vel = ((normalize(posadjust) * dot(normalize(vel), normalize(posadjust))) - vel) * dt;
				//same effect
				//vel = normalize(((normalize(posadjust) * dot(normalize(vel), normalize(posadjust))) - vel)) * length(vel);
				

				//vel += posadjust;// / dt * 0.5f;
			}
			if (ceiling && playerVel.y > 0)
				vel.y = 0;
		}
	}
	else
	{//no collision = in air
		grounded = false;
	}
	//End of collision adjustement


	if (freecam == false || specingThis == true)
	{
		cam->setCam(pos);
		rotatePlayer(oldDir, dir);
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
		this->footstepsCountdown = 20;
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
	PLAYERMSG msg = NONE;

	modifiersGetData(dt); //Dont Remove Please!

	vec3 olddir = cam->getDir();
	if (isLocalPlayer) // even if we are the local player we can be dead and spectating some one
	{
		if (i->justPressed(GLFW_KEY_ENTER))
		{
			if (lockControls)
				lockControls = false;
			else
				lockControls = true;
		}

		//lokal player anim handle
		anim_first_current = AnimationState::first_primary_idle;
		anim_third_current = AnimationState::third_idle;

		if (noclip)
			vel *= 0;

		//grounded printf
		//if (grounded)
		//	printf("ground \n");
		//else
		//	printf("air \n");

		//friction
		//if (grounded)
		//{
		//	vel -= vec3(vel.x, 0, vel.z) * dt * 30.0f;
		//}

		//instant stop
		if (grounded)
		{
			vel = vec3(0);
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
				dir = cam->getDir();
				vec2 tempvec = vec2(0, 0);

				if (grounded)
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
					}
				}

				role.update(dt);		//Temp?

				if (i->justPressed(GLFW_KEY_R))
				{
					reloadCurrentWeapon();
				}

				if (i->justPressed(GLFW_KEY_1))
				{
					role.swapWeaponLocal(0);
					msg = WPNSWITCH;
					//play anim
					if (checkAnimOverwrite(anim_first_current, AnimationState::first_primary_switch))
						anim_first_current = AnimationState::first_primary_switch;
				}

				if (i->justPressed(GLFW_KEY_2))
				{
					role.swapWeaponLocal(1);
					msg = WPNSWITCH;
					//play anim
					if (checkAnimOverwrite(anim_first_current, AnimationState::first_primary_switch))
						anim_first_current = AnimationState::first_primary_switch;
				}

				if (i->getKeyInfo(GLFW_MOUSE_BUTTON_LEFT))		//Temp
				{
					if (role.getCurrentWeapon()->shoot())
					{
						if (role.getCurrentWeapon()->getCurrentAmmo() == 0)
						{
							reloadCurrentWeapon();
						}
						else
						{
							msg = SHOOT;
							if (role.getWeaponNRequiped() == 0) //main weapon
							{
								if (checkAnimOverwrite(anim_first_current, AnimationState::first_primary_fire))
									anim_first_current = AnimationState::first_primary_fire;
							}
							else // secondary fire
							{
								if (checkAnimOverwrite(anim_first_current, AnimationState::first_secondary_fire))
									anim_first_current = AnimationState::first_secondary_fire;
							}
						}
					}
				}

				if (i->justPressed(GLFW_KEY_Q))
				{
					Consumable* c = role.getConsumable();
					if (c->use())
					{
						msg = USEITEM;
						if (checkAnimOverwrite(anim_first_current, AnimationState::first_primary_throw))
							anim_first_current = AnimationState::first_primary_throw;
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

				if (i->justPressed(GLFW_KEY_O))
				{
					msg = DEATH;
					isDead = true;
					respawnTimer = respawnTime;
				}
				if (i->justPressed(GLFW_KEY_M))
					role.setSpecialMeter(100.0f);
			} // end of player input
		} // end of lock control check

		if (role.getHealth() == 0 && !isDead)
		{
			isDead = true;
			msg = DEATH;
			respawnTimer = respawnTime;
			vel = glm::vec3(0, 0, 0);
		}

		if (isDead && respawnTimer != 0.0f)
		{
			respawnTimer -= dt;
			if (respawnTimer < FLT_EPSILON)
			{
				respawnTimer = 0.0f;
				msg = PLAYERRESPAWN;
				//role.returnToLife();
			}
		}
		movePlayer(dt, olddir, freecam, spectatingThisPlayer); //This moves the player regardless of what we might end up colliding with
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

		if (role.getHealth() == 0)
		{
			isDead = true;
			vel = glm::vec3(0, 0, 0);
		}
	}
	clearCollisionNormals(); //Doesn't actually clear the array, just manually sets size to 0. This is to speed things up a little.
	return msg;
}

void Player::movementUpdates(float dt, bool freecam, bool spectatingThisPlayer, bool spectating)
{
	/*
	This update loop has information about collisions, and as such will place the player in a legal position while maintaining frame-perfect collision data
	aka we are no longer 1 frame desynced

	In this update, we also take care of external forces such as explosions and gravity
	*/
	if (isLocalPlayer)
	{
		//ignore if we are spectating
		if (currentTeam != 0)
		{
			modifiersSetData(dt);	//Dont Remove Again Please!

			float lastHeight = pos.y;

			//sets player rotations and cam

		   // --- Animation checks ---

			bool animGroundedLast = animGrounded;

			if (grounded == false)
				animAirTimer += dt;
			else
			{
				animAirTimer = 0;
				animGrounded = true;
			}

			if (animAirTimer > 0.3f)
				animGrounded = false;

			if (animGrounded)
			{
				//Run checks
				if (vel.x * vel.x > 1 || vel.z * vel.z > 1)
				{
					if (checkAnimOverwrite(anim_first_current, AnimationState::first_primary_run))
						anim_first_current = AnimationState::first_primary_run;

					if (i->getKeyInfo(GLFW_KEY_A) && !i->getKeyInfo(GLFW_KEY_W) && !i->getKeyInfo(GLFW_KEY_S)) // strage left
					{
						if (checkAnimOverwrite(anim_third_current, AnimationState::third_strafe_left))
							anim_third_current = AnimationState::third_strafe_left;
					}
					else if (i->getKeyInfo(GLFW_KEY_D) && !i->getKeyInfo(GLFW_KEY_W) && !i->getKeyInfo(GLFW_KEY_S)) //strafe right
					{
						if (checkAnimOverwrite(anim_third_current, AnimationState::third_strafe_right))
							anim_third_current = AnimationState::third_strafe_right;
					}
					//check if we are running backwards
					else if (dot(vel, dir) < 0)
					{
						if (checkAnimOverwrite(anim_third_current, AnimationState::third_run_rev))
							anim_third_current = AnimationState::third_run_rev;
					}
					else //run forward
					{
						if (checkAnimOverwrite(anim_third_current, AnimationState::third_run))
							anim_third_current = AnimationState::third_run;
					}
				}
			}
			else //if in air
			{
				if (checkAnimOverwrite(anim_third_current, AnimationState::third_air))
					anim_third_current = AnimationState::third_air;
			}

			//Jump Checks
			if (animGrounded != animGroundedLast) //grounded changed this frame
			{
				if (animGrounded) //landed
				{
					if (checkAnimOverwrite(anim_third_current, AnimationState::third_jump_end))
						anim_third_current = AnimationState::third_jump_end;
				}
				else // jump begin
				{
					if (checkAnimOverwrite(anim_third_current, AnimationState::third_jump_begin))
						anim_third_current = AnimationState::third_jump_begin;
				}
			}
		}

		//frame peak overide
		if (checkAnimOverwrite(anim_first_framePeak, anim_first_current))
			anim_first_framePeak = anim_first_current;

		if (checkAnimOverwrite(anim_third_framePeak, anim_third_current))
			anim_third_framePeak = anim_third_current;
	}

	if (spectatingThisPlayer == true)
	{
		cam->setCam(pos, dir);
	}

	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y; //head offset. player objects have their origo at their feet
	worldMat[2].w = pos.z;

	if (freecam == true && spectatingThisPlayer == false)
	{
		worldMat[1].w -= 1.55f;  // move down if 3rd person render
	}

	if (freecam == false && isLocalPlayer == false && spectatingThisPlayer == false)
	{
		worldMat[1].w -= 1.55f; // move down if 3rd person render
	}
}

void Player::rotatePlayer(vec3 olddir, vec3 newdir)
{
	float angle = atan2(newdir.x, newdir.z) - atan2(olddir.x, olddir.z);
	rotate(0, -angle, 0);
	dir = newdir;
}

void Player::reloadCurrentWeapon()
{
	if (GetSound())
	{
		if (this->role.getRole() == 0)
			GetSound()->playUserGeneratedSound(SOUNDS::soundEffectTrapperReload);
	}

	role.getCurrentWeapon()->reload();
	//play anim
	if (checkAnimOverwrite(anim_first_current, AnimationState::first_primary_reload))
		anim_first_current = AnimationState::first_primary_reload;
	role.getCurrentWeapon()->reload();
}

Weapon* Player::getPlayerCurrentWeapon()
{
	return role.getCurrentWeapon();
}

void Player::switchWpn(WEAPON_TYPE ws, int swapLoc)
{
	role.swapWeapon(ws, swapLoc);
}

void Player::hitByBullet(Bullet* b, int newHPtotal)
{
	/*
	This is where we will extract additional modifiers from the Bullet, when applicable.
	*/
	if (newHPtotal == -1) //We are actually taking damage on the server now
	{
		int dmg = b->getDamage();
		role.takeDamage(dmg);
	}
	else //We are on a client, and thus are only interested on our HP on the server
	{
		role.setHealth(newHPtotal);
	}

	if (b->getType() == BULLET_TYPE::HACKING_DART)
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
		int dmg = e->getDamage();
		role.takeDamage(dmg);
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
	}
	myModifiers[myModifiers.size() - 1]->init(this);
}

void Player::setRole(Role role)
{
	cleanseModifiers(true);
	this->role = role;
	this->role.chooseRole(TRAPPER);
}

void Player::respawn(glm::vec3 respawnPos, glm::vec3 _dir)
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
}

void Player::healing(int amount)
{
	role.setHealth(role.getHealth() + amount);
}

bool Player::getIfHacked()
{
	bool hacked = false;

	for (int i = 0; i < myModifiers.size(); i++)
	{
		if (myModifiers[i]->getType() == MODIFIER_TYPE::HACKINGDARTMODIFIER)
			hacked = true;
	}

	return hacked;
}

glm::mat4 Player::getFPSmat()
{
	mat4 ret = glm::lookAt(cam->getPos(), cam->getPos() + cam->getDir() * -1.0f, vec3(0, 1, 0));
	ret[0].w += cam->getPos().x;
	ret[1].w += cam->getPos().y;
	ret[2].w += cam->getPos().z;
	return ret;
}