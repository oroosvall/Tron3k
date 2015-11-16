#include "Player.h"

Player::Player()
{
	
}

void Player::init(std::string pName, glm::vec3 initPos, bool isLocal)
{
	name = pName;
	pos = initPos;

	isLocalPlayer = isLocal;

	i = Input::getInput();
	cam = CameraInput::getCam();
}

void Player::setName(std::string newName)
{
	name = newName;
}

void Player::setGoalPos(glm::vec3 newPos)
{
	goalpos = newPos;
	pos = newPos; //Temporary to print new positions
}

void Player::update(float dt)
{
	/*Input* i = Input::getInput();
	if (i->getKeyInfo(GLFW_KEY_W))
		pos.z += 1.0f*dt;*/
	if (isLocalPlayer)
	{
		cam->update(dt, false);
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

		cam->setCam(pos, dir);
	}
	
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y - 2;
	worldMat[2].w = pos.z;
	
}