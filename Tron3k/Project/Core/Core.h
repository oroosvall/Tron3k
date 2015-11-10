#ifndef CORE_H
#define CORE_H

#include "Input.h"
#include "sound\SoundPlayer.h"
#include "ui\Ui.h"

#include <GLFW\glfw3.h>

#include "../Network/Client.h"
#include "../Network/Server.h"

#include "../../Engine/IEngine.h"
#include "Game\Game.h"

#include "Input.h"
#include "Console.h"

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

	GLFWwindow* win;

	Topology* top;
	Game* game;
	IEngine* engine;

	Console console;

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

	SoundPlayer musicPlayer;
public:
	void init();
	void update(float dt);
};

#endif