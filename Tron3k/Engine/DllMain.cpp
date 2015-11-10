#include "dllInclude.h"
#include "Engine.h"

IEngine* CreateEngine()
{
	return new Engine();
}

