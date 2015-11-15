#ifndef CORE_H
#define CORE_H

#include "Input.h"
#include "sound\SoundPlayer.h"
#include "ui\Ui.h"

#undef APIENTRY

#include <GLFW\glfw3.h>

#include "../Network/Client.h"
#include "../Network/Server.h"

#include "../../RenderPipeline/IRenderPipeline.h"
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
	float timepass;	//temp

	Topology* top;
	Game* game;
	IRenderPipeline* renderPipe;

	Console console;

	// window
	GLFWwindow* win;
	bool recreate;
	bool fullscreen;
	int winX, winY;

	//gamestates
	Gamestate current;
	Gamestate last;
	int subState;
	int lastSubState;
	float tick_timer;

	//gamestate update functions
	void upStart(float dt);
	void upMenu(float dt);
	void upRoam(float dt);
	void upClient(float dt);

	void clientHandleCmds(float dt); //Handles clientside /-commands

	void upServer(float dt);

	void createWindow(int x, int y, bool fullscreen);
	void removeWindow();

	void initPipeline();
	

	SoundPlayer musicPlayer;
public:
	void init();
	void update(float dt);

	void setfps(int fps);

	~Core();
};

#endif