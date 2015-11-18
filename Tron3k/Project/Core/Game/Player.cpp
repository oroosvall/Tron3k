#include "Player.h"

Player::Player()
{
	
}

void Player::init(std::string pName, glm::vec3 initPos, bool isLocal)
{
	mainWeapon.init(36, WEAPON_TYPE::PULSE_RIFLE, 0.5);

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
	pos = newPos; //Temporary
}

void Player::setGoalDir(glm::vec3 newDir)
{
	goaldir = newDir;
	rotatePlayer(dir, goaldir);
	dir = newDir; //Temporary 
}

PLAYERMSG Player::update(float dt)
{
	PLAYERMSG msg = NONE;
	bool ableToShoot = false;

	if (isLocalPlayer)
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
			vec3 olddir = cam->getDir();
			cam->update(dt, false);
			mainWeapon.update(dt);		//Temp;
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
			if (i->getKeyInfo(GLFW_KEY_R))
			{
				mainWeapon.reload();
			}

			if (i->getKeyInfo(GLFW_MOUSE_BUTTON_LEFT))		//Temp
			{
				ableToShoot = mainWeapon.shoot();
				if(ableToShoot)
				msg = SHOOT;
			}

			cam->setCam(pos, dir);
			if (olddir != dir)
				rotatePlayer(olddir, dir);
		}
	}
	
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y-0.6f;
	worldMat[2].w = pos.z;

	return msg;
}

void Player::rotatePlayer(vec3 olddir, vec3 newdir)
{
	//vec3 oldIgnoreY = vec3(olddir.x, 0, olddir.z);
	//vec3 newIgnoreY = vec3(newdir.x, 0, newdir.z);

	//float angle = (dot(oldIgnoreY, newIgnoreY)) / (oldIgnoreY.length() * newIgnoreY.length());
	float angle = atan2(newdir.x, newdir.z) - atan2(olddir.x, olddir.z);
	rotate(0, -angle, 0);
}

void Player::getWeaponData(WEAPON_TYPE &wpntype)
{
	wpntype = mainWeapon.getType();
}