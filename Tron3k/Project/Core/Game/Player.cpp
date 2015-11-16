#include "Player.h"

Player::Player()
{

}

void Player::init(std::string pName, glm::vec3 initPos)
{
	name = pName;
	pos = initPos;

	worldMat[0].x = 0.02;
	worldMat[1].y = 0.02;
	worldMat[2].z = 0.02;

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

	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y;
	worldMat[2].w = pos.z;

	/*Input* i = Input::getInput();
	if (i->getKeyInfo(GLFW_KEY_W))
		pos.z += 1.0f*dt;*/
}