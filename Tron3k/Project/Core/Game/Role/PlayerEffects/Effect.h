#ifndef EFFECT_H
#define EFFECT_H

#include <glm/glm.hpp>

class Effect
{
protected:
	int playerId;
	int effectId;
	glm::vec3 pos;
public:
	virtual void init(int pid, int eid, glm::vec3 position) = 0;
	void getId(int &pid, int &eid) { pid = playerId; eid = effectId; };
	virtual int update(float dt) = 0;
};
#endif