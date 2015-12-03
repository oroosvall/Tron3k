#ifndef EFFECT_H
#define EFFECT_H

class Effect
{
protected:
public:
	virtual void init() = 0;
	virtual int update(float dt) = 0;
};
#endif