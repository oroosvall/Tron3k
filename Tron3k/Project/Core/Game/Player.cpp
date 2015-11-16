#include "Player.h"

Player::Player()
{

}

void Player::init(std::string pName, glm::vec3 initPos)
{
	name = pName;
	pos = initPos;
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
}