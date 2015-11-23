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