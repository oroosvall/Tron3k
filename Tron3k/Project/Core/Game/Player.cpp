#include "Player.h"

Player::Player()
{
	
}

void Player::init(std::string pName, glm::vec3 initPos, bool isLocal)
{
	weapons[0].init(36, WEAPON_TYPE::PULSE_RIFLE, 0.5);
	weapons[1].init(80, WEAPON_TYPE::POOP_GUN, 0.1);

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
	glm::vec3 playerVel = vel*speed;
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
		
		if (!lockControls)
		{
			//move camera to where we are looking.
			//if freecam is true the cam can move on its own
			vec3 olddir = cam->getDir();
			if(spectating == false)
				cam->update(dt, freecam);

			//If freecam or spectating dont take player move input
			if (freecam == false)
			{
				dir = cam->getDir();
				bool stop = true;
				if (i->getKeyInfo(GLFW_KEY_W))
				{
					vec3 forward = normalize(vec3(dir.x, 0, dir.z));
					if (grounded)
					{
						vel.x = forward.x;
						vel.z = forward.z;
						stop = false;
					}
				}

				if (i->getKeyInfo(GLFW_KEY_S))
				{
					vec3 back = -normalize(vec3(dir.x, 0, dir.z));
					if (grounded)
					{
						vel.x = back.x;
						vel.z = back.z;
						stop = false;
					}
				}

				if (i->getKeyInfo(GLFW_KEY_A))
				{
					vec3 left = cross(vec3(0, 1, 0), dir);
					left = normalize(left);
					if (grounded)
					{	
						vel.x = left.x;
						vel.z = left.z;
						stop = false;
					}
				}
				if (i->getKeyInfo(GLFW_KEY_D))
				{
					vec3 right = cross(dir, vec3(0, 1, 0));
					right = normalize(right);
					if (grounded)
					{
						vel.x = right.x;
						vel.z = right.z;
						stop = false;
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
						vel.y = 4.0f;
						grounded = false;
					}
				}

				movePlayer(dt);

				weapons[currentWpn].update(dt);		//Temp;
				if (i->justPressed(GLFW_KEY_R))
				{
					weapons[currentWpn].reload();
				}

				if (i->justPressed(GLFW_KEY_1))
				{
					currentWpn = 0;
					msg = WPNSWITCH;
				}

				if (i->justPressed(GLFW_KEY_2))
				{
					currentWpn = 1;
					msg = WPNSWITCH;
				}

				if (i->getKeyInfo(GLFW_MOUSE_BUTTON_LEFT))		//Temp
				{
					ableToShoot = weapons[currentWpn].shoot();
					if (ableToShoot)
						msg = SHOOT;
				}

				if (i->justPressed(GLFW_KEY_M))					//Temp?
				{
					GetSound()->enableSounds();
				}	
			} // end of player input

			if (freecam == false || spectatingThisPlayer == true)
			{
				cam->setCam(pos);
				rotatePlayer(olddir, dir);
			}
		} // end of lock control check
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
	return &weapons[currentWpn];
}

void Player::switchWpn(WEAPON_TYPE ws)
{
	/*
	THIS CODE IS TEMPORARY
	This logic needs to be moved to the class, where we can easily compare to the class' weapons. If the class does not hold any of these weapons, we can conclude that the weapon switch
	is due to a tech pickup or super weapon. Then, we check which type of weapon ws is and figure out in which slot to put it.
	*/
	switch (ws)
	{
	case WEAPON_TYPE::PULSE_RIFLE:
		currentWpn = 0;
		break;
	case WEAPON_TYPE::POOP_GUN:
		currentWpn = 1;
		break;
	}
}

void Player::hitByBullet(BulletHitInfo hi, glm::vec3 dir)
{
	/*
	
	THIS CODE IS TEMPORARY
	This needs to instead draw information from the templated bullets that are supposed to exist globally
	Right now, we hardcode boys

	*/
	switch (hi.bt)
	{
	case BULLET_TYPE::PULSE_SHOT:
		pos += dir;
		break;
	case BULLET_TYPE::POOP:
		pos += dir*0.1f;
		break;
	}
}

void Player::applyGravity(Physics* p, float dt)
{
	p->addGravity(vel, dt);
	if (pos.y <= -2.0f) // Temp code for floor lol
	{
		vel.y = 0.0f;
		pos.y = -2.0f;
		grounded = true;
	}
}