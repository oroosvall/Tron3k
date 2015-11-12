#ifndef ENGINE_H
#define ENGINE_H

#include "IRenderPipeline.h"

class RenderPipeline : public IRenderPipeline
{
private:

public:
	virtual bool init();
	virtual void release();
	virtual void update();
	virtual void render();
};


#endif