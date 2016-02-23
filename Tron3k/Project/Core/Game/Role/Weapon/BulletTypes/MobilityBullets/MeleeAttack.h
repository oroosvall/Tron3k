#ifndef MELEEATTACK_H
#define MELEEATTACK_H

#include <glm/glm.hpp>
#include "../../Bullet.h"
#include "../../../../Player.h"

#include "../../../../GameDataIndex.h"

class MeleeAttack : public Bullet
{
private:
	Player* target;
	float attackTimer = 0.2f;
	bool teleported = false;
public:
	MeleeAttack(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID, Player* player);
	~MeleeAttack();

	int update(float dt);

};
#endif