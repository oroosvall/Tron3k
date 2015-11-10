#ifndef ENGINE_H
#define ENGINE_H

#include "IEngine.h"

class Engine : public IEngine
{
private:

public:
	virtual void release();
	virtual void render();
};


#endif