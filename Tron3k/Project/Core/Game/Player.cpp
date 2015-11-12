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