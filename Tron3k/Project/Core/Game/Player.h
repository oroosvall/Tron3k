#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Animation.h"

#include <glm/glm.hpp>
#include <string>

class Player : public GameObject
{
private:
	std::string name;
	glm::vec3 pos;
public:
	Player();
	void init(std::string name, glm::vec3 pos);

	void setName(std::string newName);

	std::string getName() { return name; };
	glm::vec3 getPos() { return pos; };
};

#endif