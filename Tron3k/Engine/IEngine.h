#ifndef IENGINE_H
#define IENGINE_H

class IEngine
{
public:
	virtual void release() = 0;
	virtual void render() = 0;

};

#endif