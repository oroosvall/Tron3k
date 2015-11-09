#include "Engine.h"
#include <iostream>

void Engine::release()
{
	// place delete code here

	delete this; // yes this is safe
}