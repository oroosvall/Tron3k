#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Animation.h"
#include "../Input.h"

#include <glm/glm.hpp>
#include <string>

class Player : public GameObject
{
private:
	std::string name;
	glm::vec3 pos;

	glm::vec3 goalpos; //Used by non-local players to move smoothly
public:
	Player();
	void init(std::string name, glm::vec3 pos);

	void update(float dt);

	void setName(std::string newName);
	void setGoalPos(glm::vec3 newPos);

	std::string getName() { return name; };
	glm::vec3 getPos() { return pos; };
};

#endif