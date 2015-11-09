#ifndef ENGINE_H
#define ENGINE_H

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec( dllexport)
#else
#define ENGINE_API __declspec( dllimport  )
#endif


class Engine
{
private:

public:
	void reneder() {};
};

extern "C" void ENGINE_API test();

#endif