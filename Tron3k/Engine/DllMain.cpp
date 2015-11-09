#include "dllInclude.h"
#include "Engine.h"
#include "Game\Game.h"

IEngine* CreateEngine()
{
	return new Engine();
}

IGame* CreateGameObject()
{
	return new Game();
}
