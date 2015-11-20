#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "../../SoundPlayer/sound/SoundPlayer.h"
#include "Animation.h"
#include "../Input.h"
#include "../CameraInput.h"

#include <glm/glm.hpp>
#include "Role/Weapon/Weapon.h"
#include <string>

enum PLAYERMSG { NONE, SHOOT, WPNSWITCH };

class Player : public GameObject
{
private:
	bool lockControls = false;


	Weapon weapons[2];
	int currentWpn = 0;

	Weapon pickup;
	Weapon super;

	std::string name;
	glm::vec3 pos;
	glm::vec3 dir; //Current viewing direction

	glm::vec3 goalpos; //Used by non-local players to move smoothly
	glm::vec3 goaldir; //Used by non-locals to turn smoothly

	bool isLocalPlayer;

	int currentTeam;

	Input* i;
	CameraInput* cam;

	void rotatePlayer(vec3 olddir, vec3 newdir);
public:
	Player();
	void init(std::string name, glm::vec3 pos, bool isLocal = false);

	PLAYERMSG update(float dt);

	void setName(std::string newName);
	void setGoalPos(glm::vec3 newPos);
	void setGoalDir(glm::vec3 newDir);

	std::string getName() { return name; };
	glm::vec3 getPos() { return pos; };
	glm::vec3 getDir() { return dir; };

	bool isLocal() { return isLocalPlayer; };

	Weapon* getPlayerCurrentWeapon();
	void switchWpn(WEAPON_TYPE ws);

	void setTeam(int teamid) { currentTeam = teamid; };
	int getTeam() { return currentTeam; };
};

#endif