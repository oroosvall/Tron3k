#include "Player.h"

Player::Player()
{
	
}

void Player::init(std::string pName, glm::vec3 initPos, bool isLocal)
{
	name = pName;
	pos = initPos;
	vel = glm::vec3(0, 0, 0);

	isLocalPlayer = isLocal;

	i = Input::getInput();
	cam = CameraInput::getCam();

	if (isLocal)
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
	//pos = newPos; //Temporary
}

void Player::setGoalDir(glm::vec3 newDir)
{
	goaldir = newDir;
	oldDir = dir;
	//dir = newDir; //Temporary 
}

void Player::movePlayer(float dt)
{
	glm::vec3 playerVel = vel*role.getMovementSpeed();
	pos += playerVel * dt; //Here we will also include external forces
}

PLAYERMSG Player::update(float dt, bool freecam, bool spectatingThisPlayer, bool spectating)
{
	PLAYERMSG msg = NONE;
	bool ableToShoot = false;

	if (isLocalPlayer) // even if we are the local player we can be dead and spectating some one
	{
		if (i->justPressed(GLFW_KEY_ENTER))
		{
			if (lockControls)
				lockControls = false;
			else
				lockControls = true;
		}

		vec3 olddir = cam->getDir();
		if (!lockControls)
		{
			//move camera to where we are looking.
			//if freecam is true the cam can move on its own
			if(spectating == false)
				cam->update(dt, freecam);

			//If freecam or spectating dont take player move input
			if (freecam == false)
			{
				dir = cam->getDir();
				bool stop = true;
				vec2 tempvec = vec2(0, 0);

				if (i->getKeyInfo(GLFW_KEY_W))
				{
					tempvec = normalize(vec2(dir.x, dir.z));
					if (grounded)
					{
						vel.x = tempvec.x;
						vel.z = tempvec.y;
						stop = false;
					}
				}

				if (i->getKeyInfo(GLFW_KEY_S))
				{
					tempvec = -normalize(vec2(dir.x, dir.z));
					if (grounded)
					{
						vel.x = tempvec.x;
						vel.z = tempvec.y;
						stop = false;
					}
				}

				if (i->getKeyInfo(GLFW_KEY_A) || i->getKeyInfo(GLFW_KEY_D))
				{
					if (!(i->getKeyInfo(GLFW_KEY_A) && i->getKeyInfo(GLFW_KEY_D)))
					{
						if (i->getKeyInfo(GLFW_KEY_A))
						{
							vec3 left = cross(vec3(0, 1, 0), dir);
							tempvec = normalize(vec2(left.x, left.z));
							if (grounded)
							{
								vel.x += tempvec.x;
								vel.z += tempvec.y;
								stop = false;
								tempvec = vec2(vel.x, vel.z);
								tempvec = normalize(tempvec);
								vel.x = tempvec.x;
								vel.z = tempvec.y;
							}
						}

						if (i->getKeyInfo(GLFW_KEY_D))
						{
							vec3 right = cross(dir, vec3(0, 1, 0));
							tempvec = normalize(vec2(right.x, right.z));
							right = normalize(right);
							if (grounded)
							{
								vel.x += tempvec.x;
								vel.z += tempvec.y;
								stop = false;
								tempvec = vec2(vel.x, vel.z);
								tempvec = normalize(tempvec);
								vel.x = tempvec.x;
								vel.z = tempvec.y;
							}
						}
					}
				}

				if (stop)
				{
					if (grounded)
					{
						vel.x = 0;
						vel.z = 0;
					}
				}

				if (i->justPressed(GLFW_KEY_SPACE))
				{
					if (grounded)
					{
						vel.y = 3.0f;
						grounded = false;
					}
				}

				role.getCurrentWeapon()->update(dt);		//Temp;

				if (i->justPressed(GLFW_KEY_R))
				{
					role.getCurrentWeapon()->reload();
				}

				if (i->justPressed(GLFW_KEY_1))
				{
					role.swapWeapon(0);
					msg = WPNSWITCH;
				}

				if (i->justPressed(GLFW_KEY_2))
				{
					role.swapWeapon(1);
					msg = WPNSWITCH;
				}

				if (i->getKeyInfo(GLFW_MOUSE_BUTTON_LEFT))		//Temp
				{
					ableToShoot = role.getCurrentWeapon()->shoot();
					if (ableToShoot)
						msg = SHOOT;
				}

				if (i->justPressed(GLFW_KEY_M))					//Temp?
				{
					GetSound()->enableSounds();
				}

				if (i->justPressed(GLFW_KEY_O))
				{
					msg = DEATH;
					isDead = true;
					respawnTimer = respawnTime;
				}
			} // end of player input
		} // end of lock control check
		movePlayer(dt); //Move the player regardless of control lock
		if (freecam == false || spectatingThisPlayer == true)
		{
			cam->setCam(pos);
			rotatePlayer(olddir, dir);
		}

		if (role.getHealth() == 0 && !isDead)
		{
			isDead = true;
			msg = DEATH;
			respawnTimer = respawnTime;
		}

		if (isDead)
		{
			respawnTimer -= dt;
			if (respawnTimer < FLT_EPSILON)
			{
				respawnTimer = 0.0f;
				msg = PLAYERRESPAWN;
				//role.returnToLife();
			}
		}

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
	}
	

	if (spectatingThisPlayer == true)
	{
		cam->setCam(pos, dir);
	}
	
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y-0.6f;
	worldMat[2].w = pos.z;

	return msg;
}

void Player::rotatePlayer(vec3 olddir, vec3 newdir)
{
	float angle = atan2(newdir.x, newdir.z) - atan2(olddir.x, olddir.z);
	rotate(0, -angle, 0);
}

Weapon* Player::getPlayerCurrentWeapon()
{
	return role.getCurrentWeapon();
}

void Player::switchWpn(WEAPON_TYPE ws)
{
	role.swapWeapon(ws);
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

	if (!isLocalPlayer)
	{
		if (role.getHealth() == 0)
		isDead = true;
	}
}

void Player::applyGravity(Physics* p, float dt)
{
	p->addGravity(vel, dt);
	/*if (pos.y <= -10.0f) // Temp code for floor lol
	{
		vel.y = 0.0f;
		pos.y = -10.0f;
		grounded = true;
	}*/
}

void Player::setRole(Role role)
{
	this->role = role;
}

void Player::respawn(glm::vec3 respawnPos)
{
	pos = respawnPos;
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y - 0.6f;
	worldMat[2].w = pos.z;

	role.returnToLife();
}