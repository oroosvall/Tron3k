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

		if (this->getFootsteps() && this->getGrounded() && GetSoundActivated() && this->role.getRole() != 1)
		{
			this->setFootstepsCountdown();
			this->setFootstepsLoop(false);
			GetSound()->playFootsteps(this->role.getRole(), pos.x, pos.y, pos.z);
		}

		if (this->role.getRole() == 1 && GetSound()->destroyerPaused == true)
		{

			GetSound()->playFootsteps(this->role.getRole(), pos.x, pos.y, pos.z);
			GetSound()->destroyerPaused = false;
		}
	}

	else
	{
		if (this->role.getRole() == 1 && GetSound()->destroyerPaused == false)
		{
			GetSound()->stopDestroyer(pos.x, pos.y, pos.z);
		}
	}



	glm::vec3 playerVel = vec3(0);
	playerVel.x = vel.x*role.getMovementSpeed();
	playerVel.z = vel.z*role.getMovementSpeed();
	playerVel.y = vel.y;
	pos += playerVel * dt; //Here we will also include external forces.. EDIT: External forces moved, for now
	vec3 posadjust = vec3(0);

	if (collisionNormalSize > 0)
	{
		if (collisionNormalSize > 1)

			int debug = 1;

		bool ceiling = false;
		for (int k = 0; k < collisionNormalSize; k++)
		{
			//push pos away and lower velocity using pendepth
			vec3 pendepth = vec3(collisionNormals[k]) * collisionNormals[k].w;
			if (collisionNormals[k].y < 0)
				ceiling = true;

			//ramp factor and grounded
			if (collisionNormals[k].y > 0)
			{
				grounded = true;
				posadjust.x += pendepth.x;
				if (posadjust.y * posadjust.y < pendepth.y * pendepth.y)
					posadjust.y = pendepth.y;
				posadjust.z += pendepth.z;
				break;
			}

			// abslut value, if two collisions from the same angle they should not move us twice the distance
			
		}

		// this is for air only since grounded will set the vel to 0 later
		// the dt * 0.5 is supposed to remove almost all velocity in that dir
		// while + posajust w/o  /dt  will remove it slower
		vel += posadjust;// / dt * 0.5f;
	}

	//get normals
	//std::vector<vec4> cNorms = physics->sphereVWorldCollision(pos - (vec3(0, 0.55f, 0)), 1);

	/*//Collision handling here, after movement
	bool ceiling = false;
	vec3 posadjust = vec3(0);

	grounded = false;

	//if we collided with something
	if (cNorms.size() > 0)
	{
		if (cNorms.size() > 1)

			int debug = 1;

		bool ceiling = false;
		for (int k = 0; k < cNorms.size(); k++)
		{
			//push pos away and lower velocity using pendepth
			vec3 pendepth = vec3(cNorms[k]) * cNorms[k].w;
			if (cNorms[k].y < 0)
				ceiling = true;

			//ramp factor and grounded
			if (cNorms[k].y > 0)
			{
				grounded = true;
			}

			// abslut value, if two collisions from the same angle they should not move us twice the distance
			posadjust.x += pendepth.x;
			if (posadjust.y * posadjust.y < pendepth.y * pendepth.y)
				posadjust.y = pendepth.y;
			posadjust.z += pendepth.z;
		}

		// this is for air only since grounded will set the vel to 0 later
		// the dt * 0.5 is supposed to remove almost all velocity in that dir
		// while + posajust w/o  /dt  will remove it slower
		vel += posadjust;// / dt * 0.5f;

		if (ceiling)
			posadjust.y = 0;

		pos += posadjust;

		if (ceiling && vel.y > 0)
			vel.y = 0;
	}

	if (freecam == false || specingThis == true)
	{
		cam->setCam(pos);
		rotatePlayer(oldDir, dir);
	}*/
}

void Player::movePlayerCollided(float dt, glm::vec3 oldDir, bool freecam, bool specingThis)
{
	//get normals
	//std::vector<vec4> cNorms = physics->sphereVWorldCollision(pos - (vec3(0, 0.55f, 0)), 1);

	//Collision handling here, after movement
	bool ceiling = false;
	vec3 posadjust = vec3(0);

	grounded = false;
	collided = false;

	//if we collided with something
	if (collisionNormalSize > 0)
	{
		collided = true;
		if (collisionNormalSize > 1)

			int debug = 1;

		bool ceiling = false;
		for (int k = 0; k < collisionNormalSize; k++)
		{
			//push pos away and lower velocity using pendepth
			vec3 pendepth = vec3(collisionNormals[k]) * collisionNormals[k].w;
			if (collisionNormals[k].y < 0)
				ceiling = true;

			//ramp factor and grounded
			if (collisionNormals[k].y > 0)
			{
				grounded = true;
			}

			// abslut value, if two collisions from the same angle they should not move us twice the distance
			posadjust.x += pendepth.x;
			if (posadjust.y * posadjust.y < pendepth.y * pendepth.y)
				posadjust.y = pendepth.y;
			posadjust.z += pendepth.z;
		}

		// this is for air only since grounded will set the vel to 0 later
		// the dt * 0.5 is supposed to remove almost all velocity in that dir
		// while + posajust w/o  /dt  will remove it slower
		vel += posadjust;// / dt * 0.5f;

		if (ceiling)
			posadjust.y = 0;

		pos += posadjust;

		if (ceiling && vel.y > 0)
			vel.y = 0;
	}

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
		this->footstepsCountdown = 2000000000000;
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
				dir = cam->getDir();
				vec2 tempvec = vec2(0, 0);

				if (!collided)//IN THE AIR YO
				{
					if (i->getKeyInfo(GLFW_KEY_W))
					{
						if (length(glm::vec2(airVelocity.x, airVelocity.z)) < role.getMovementSpeed())
						{
							airVelocity += normalize(glm::vec3(dir.x, 0, dir.z))*dt*0.5f;
						}
					}

					if (i->getKeyInfo(GLFW_KEY_S))
					{
						if (length(glm::vec2(airVelocity.x, airVelocity.z)) > -role.getMovementSpeed())
						{
							airVelocity -= normalize(glm::vec3(dir.x, 0, dir.z))*dt*0.5f;
						}
					}

					if (!(i->getKeyInfo(GLFW_KEY_A) && i->getKeyInfo(GLFW_KEY_D)))
					{
						if (i->getKeyInfo(GLFW_KEY_A))
						{
							vec3 left = cross(vec3(0, 1, 0), dir);
							if (length(left) > 0)
							{
								airVelocity += normalize(left)*dt;
							}
						}
						if (i->getKeyInfo(GLFW_KEY_D))
						{
							vec3 right = cross(dir, vec3(0, 1, 0));
							if (length(right) > 0)
							{
								airVelocity += normalize(right)*dt;
							}
						}
					}

					vel.x = airVelocity.x;
					vel.z = airVelocity.z;
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
					}
				}

				role.update(dt);		//Temp?

				if (i->justPressed(GLFW_KEY_R))
				{
					if (role.getCurrentWeapon()->getIfReloading())
					{
						reloadCurrentWeapon();
					}
				}

				if (i->justPressed(GLFW_KEY_1))
				{
					role.swapWeaponLocal(0);
					msg = WPNSWITCH;
					animOverideIfPriority(anim_first_current, AnimationState::first_primary_switch);
				}

				if (i->justPressed(GLFW_KEY_2))
				{
					role.swapWeaponLocal(1);
					msg = WPNSWITCH;
					animOverideIfPriority(anim_first_current, AnimationState::first_primary_switch);
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
							shoot();
						}
					}
				}

				if (i->justPressed(GLFW_KEY_Q))
				{
					if (role.getCurrentWeapon()->getIfReloading())
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

				if (i->justPressed(GLFW_KEY_O))
				{
					role.setHealth(0);
				}
				if (i->justPressed(GLFW_KEY_M))
					role.setSpecialMeter(100.0f);
			} // end of player input
		} // end of lock control check

		if (pos.y < -15.0f && isLocalPlayer)	//If we fall through the map we die
		{
			role.setHealth(0);
		}

		if (role.getHealth() <= 0 && !isDead)
		{
			isDead = true;
			msg = DEATH;
			respawnTimer = respawnTime;
			vel = glm::vec3(0, 0, 0);
			animOverideIfPriority(anim_third_current, AnimationState::third_primary_death);
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

		movementAnimationChecks(dt);

		modifiersSetData(dt);	//Dont Remove Again Please!
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

	vec3 olddir = cam->getDir();
	if (isLocalPlayer)
	{
		//ignore if we are spectating
		if (currentTeam != 0)
		{
			movePlayerCollided(dt, olddir, freecam, spectatingThisPlayer);
			if (!freecam)
				cam->setCam(pos, dir);

			float lastHeight = pos.y;

			//sets player rotations and cam

		}
	}
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
	if (!role.getCurrentWeapon()->getIfFullAmmo())
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

	//Add a bullet recoil factor that is multiplied by a random number and smooth it out
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

	setAnimState_f_c(AnimationState::first_primary_idle);
	setAnimState_f_p(AnimationState::first_primary_idle);
	setAnimState_t_c(AnimationState::third_primary_idle);
	setAnimState_t_p(AnimationState::third_primary_idle);

	/*anim_first_current = AnimationState();
	anim_first_framePeak= AnimationState();
	anim_third_current = AnimationState();
	anim_third_framePeak = AnimationState();*/

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

void Player::movementAnimationChecks(float dt)
{
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
	
	//default movement anims
	anim_first_current = AnimationState::first_primary_idle;
	anim_third_current = AnimationState::third_primary_idle;

	if (grounded)
	{
		//Run checks
		if (vel.x != 0 || vel.z != 0)
		{
			//first person run
			animOverideIfPriority(anim_first_current, AnimationState::first_primary_run);

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
				animOverideIfPriority(anim_third_current, AnimationState::third_primary_run);
		}
	}
	else //if in air
		animOverideIfPriority(anim_third_current, AnimationState::third_primary_air);

	//Jump Checks
	if (grounded != animGroundedLast) //grounded changed this frame
	{
		if (grounded) //landed
			animOverideIfPriority(anim_third_current, AnimationState::third_primary_jump_end);

		else // jump begin
			animOverideIfPriority(anim_third_current, AnimationState::third_primary_jump_begin);
	}

	animGroundedLast = grounded;

	//frame peak overide
	if (checkAnimOverwrite(anim_first_framePeak, anim_first_current))
		anim_first_framePeak = anim_first_current;

	if (checkAnimOverwrite(anim_third_framePeak, anim_third_current))
		anim_third_framePeak = anim_third_current;
}