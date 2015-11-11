#include "Engine.h"
#include <iostream>

void Engine::init()
{

}

void Engine::release()
{
	// place delete code here

	delete this; // yes this is safe
}

void Engine::update()
{

}

void Engine::render()
{

}


IEngine* CreateEngine()
{
	return new Engine();
}