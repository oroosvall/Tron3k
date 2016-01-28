#ifndef EFFECT_H
#define EFFECT_H

#include <glm/glm.hpp>

#include "../../GameDataIndex.h"

class Effect
{
protected:
	glm::vec3 pos;

	int playerId;
	int effectId;
	EFFECT_TYPE type;
public:
	// orka göra funktion
	glm::vec3 getPos() { return pos; };
	glm::vec3 setPos(glm::vec3 p) { pos = p; };

	virtual void init(int pid, int eid, glm::vec3 position) = 0;
	void getId(int &pid, int &eid) { pid = playerId; eid = effectId; };
	virtual int update(float dt) = 0;

	virtual int getDamage() { return 0; };
	virtual void setDamage(int dmg) {};
	virtual void setInterestingVariable(float f) {};
	virtual float getInterestingVariable() { return -1; };

	EFFECT_TYPE getType() { return type; };
};
#endif