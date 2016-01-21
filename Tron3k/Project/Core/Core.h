#ifndef CORE_H
#define CORE_H

#include "Input.h"
#include "../../SoundPlayer/sound/SoundPlayer.h"
#include "ui\Ui.h"

#undef APIENTRY

#include <GLFW\glfw3.h>

#include "../Network/Client.h"
#include "../Network/Server.h"

#include "../../RenderPipeline/IRenderPipeline.h"
#include "Game\Game.h"

#include "Input.h"
#include "Console.h"

#include "CameraInput.h"

#include "ui\UIManager.h"

#include <fstream> //load settings
#include <sstream>




class Core
{
private:
	float timepass;	//temp
	Topology* top;
	Game* game;
	IRenderPipeline* renderPipe;
	Input* i;
	Console console;

	CameraInput* serverCam;

	UIManager* uiManager;

	// window
	GLFWwindow* win;
	bool recreate;
	bool fullscreen;
	bool serverRender;
	int winX, winY;

	//gamestates
	Gamestate current;
	Gamestate last;
	int subState;
	int lastSubState;

	//network
	float tick_timer;

	//netlog
	bool client_record;
	bool client_playback;
	float playbackSpeed = 1.0f;

	//gamestate update functions
	void upStart(float dt);
	void upMenu(float dt);
	void upRoam(float dt);
	void upClient(float dt);
	void upServer(float dt);

	void startHandleCmds();
	void roamHandleCmds();
	void clientHandleCmds(); 
	void serverHandleCmds();

	void renderWorld(float dt);
	void createWindow(int x, int y, bool fullscreen);
	void removeWindow();

	void initPipeline();

	void disconnect();

	//load from file
	IpAddress _addrs;
	int _port;
	string _name;
	void loadSettings();
	void saveSettings();

	//Relays BoundingBoxes to Physics
	bool sendChunkBoxes(int chunkID);
	void sendPlayerBox();
	void sendWorldBoxes();

public:
	void init();
	void update(float dt);

	bool windowVisible() const;

	void setfps(int fps);

	~Core();
};

#endif