#ifndef IENGINE_H
#define IENGINE_H

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec( dllexport)
#else
#define ENGINE_API __declspec( dllimport  )
#endif

class IEngine
{
public:
	virtual void init() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

};

extern "C" ENGINE_API IEngine* CreateEngine();

#endif