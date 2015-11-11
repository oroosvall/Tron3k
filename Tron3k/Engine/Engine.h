#ifndef ENGINE_H
#define ENGINE_H

#include "IEngine.h"

class Engine : public IEngine
{
private:

public:
	virtual void init();
	virtual void release();
	virtual void update();
	virtual void render();
};


#endif