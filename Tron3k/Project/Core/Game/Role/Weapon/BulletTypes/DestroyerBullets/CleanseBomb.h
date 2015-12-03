#ifndef CLEANSEBOMB_H
#define CLEANSEBOMB_H

#include <glm/glm.hpp>
#include "../../Bullet.h"

#include "../../../../GameDataIndex.h"

class CleanseBomb : public Bullet
{
private:
public:
	CleanseBomb(glm::vec3 pos, glm::vec3 dir, int pID, int bID, int tID);
	~CleanseBomb();

	int update(float dt);

};
#endif