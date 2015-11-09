#ifndef CORE_H
#define CORE_H

#include "Input.h"
#include "sound\Sound.h"
#include "ui\Ui.h"

#include "../Network/Client.h"
#include "../Network/Server.h"

#include "../../Engine/IEngine.h"
#include "../../Engine/Game/IGame.h"

enum Gamestate
{
	START,
	MENU,
	ROAM,
	CLIENT,
	SERVER
};


class Core
{
private:
	Topology* top;
	IGame* game;
	IEngine* engine;

	//gamestates
	Gamestate current;
	Gamestate last;
	int subState;
	int lastSubState;

	//gamestate update functions
	void upStart(float dt);
	void upMenu(float dt);
	void upRoam(float dt);
	void upClient(float dt);
	void upServer(float dt);
public:
	void init();
	void update(float dt);
};

#endif