#include "Player.h"

Player::Player()
{
	
}

void Player::init(std::string pName, glm::vec3 initPos, bool isLocal)
{
	role.chooseRole(TRAPPER);

	name = pName;
	pos = initPos;

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
		
		/*
		GRAVITY????
		*/

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

				if (i->getKeyInfo(GLFW_KEY_W))
					pos += dir * dt;

				if (i->getKeyInfo(GLFW_KEY_S))
					pos -= dir * dt;

				if (i->getKeyInfo(GLFW_KEY_A))
				{
					vec3 left = cross(vec3(0, 1, 0), dir);
					left = normalize(left);
					pos += left * dt;
				}
				if (i->getKeyInfo(GLFW_KEY_D))
				{
					vec3 left = cross(dir, vec3(0, 1, 0));
					left = normalize(left);
					pos += left * dt;
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
	return role.getCurrentWeapon();
}

void Player::switchWpn(WEAPON_TYPE ws)
{
	role.swapWeapon(ws);
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
	p->addGravity(pos, dt);
}