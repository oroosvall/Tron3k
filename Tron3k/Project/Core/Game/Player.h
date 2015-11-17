#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Animation.h"
#include "../Input.h"
#include "../CameraInput.h"

#include <glm/glm.hpp>
#include <string>

class Player : public GameObject
{
private:
	std::string name;
	glm::vec3 pos;
	glm::vec3 dir; //Current viewing direction

	glm::vec3 goalpos; //Used by non-local players to move smoothly
	glm::vec3 goaldir; //Used by non-locals to turn smoothly

	bool isLocalPlayer;

	Input* i;
	CameraInput* cam;
public:
	Player();
	void init(std::string name, glm::vec3 pos, bool isLocal = false);

	void update(float dt);

	void setName(std::string newName);
	void setGoalPos(glm::vec3 newPos);
	void setGoalDir(glm::vec3 newDir);

	std::string getName() { return name; };
	glm::vec3 getPos() { return pos; };
};

#endif