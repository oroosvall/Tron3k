#ifndef CORE_H
#define CORE_H


#include "ui\UIManager.h"
#include "Input.h"
#include "../../SoundPlayer/sound/SoundPlayer.h"

#undef APIENTRY

#include "../Network/Client.h"
#include "../Network/Server.h"

#include "../../RenderPipeline/IRenderPipeline.h"
#include "Game\Game.h"

#include "Input.h"
#include "Console.h"

#include "CameraInput.h"

#include <fstream> //load settings
#include <sstream>

#define VALIDKEYSIP 11
#define VALIDKEYSNAME 29
#define VALIDKEYSENS 10

class Core
{
private:

	const std::string title = "Neon Dawn";

	bool settingsLoaded = false;
	bool controlsLoaded = false;
	int uitmpcounter;
	float slowmode = 1.0f;
	float slowdownFactor = 0.8f;
	float slowdownTimer = 0.0f;

	float timepass;	//temp
	Topology* top;
	Game* game;
	IRenderPipeline* renderPipe;
	Input* i;
	Console console;
	bool cheatsOn = false;

	CameraInput* serverCam;

	UIManager* uiManager;
	bool renderUI;
	KOTHSTATE kothState;
	
	int* namePlates;
	float cursorBlink;

	bool cursorInvisible;

	// window
	GLFWwindow* win;
	bool recreate;
	bool fullscreen;
	bool serverRender;
	int winX, winY;

	bool closeGame;

	//gamestates
	Gamestate current;
	Gamestate last;
	int subState;
	int lastSubState;
	bool firstTimeInEnd;
	bool lowTicketsFirstTime;
	bool firstTimeInWarmUp;

	//network
	float tick_timer;

	int leaderBoardTextID;
	int leaderBoardSmallTextID;

	//netlog
	bool client_record;
	bool client_playback;
	float playbackSpeed = 1.0f;

	//for culling
	vec3 lastCampos;
	vec3 lastPlayerPos;

	//gamestate update functions
	void upStart(float dt);
	void upMenu(float dt);
	void upRoam(float dt);
	void upClient(float dt);
	void upServer(float dt);

	
	void startHandleCmds(std::string com = "");
	void roamHandleCmds(std::string com = "");
	void clientHandleCmds(std::string com = "");
	void serverHandleCmds();

	void renderWorld(float dt);
	void renderIntoFly(float dt);
	void inGameUIUpdate();
	void handleCulling();
	void scoreboardRender();
	void minimapRender();
	void trailQuadsRender(int hackedTeam);
	void effectsRender(int hackedTeam);

	void createWindow(int x, int y, bool fullscreen);
	void removeWindow();

	void initPipeline();

	void disconnect();

	int introFlyIndex = -1;
	float introFlyTimer = 0;
	float introFlyTimerStart;
	vec3 startFly;
	vec3 endFly;
	vec3 flyCamDir;

	//load from file
	IpAddress _addrs;
	int _port;
	string _name;
	void loadSettings();
	void saveSettings();

	void loadControls();
	void saveControls();

	//Relays BoundingBoxes to Physics
	bool sendChunkBoxes(int chunkID);
	void sendCapPointBoxes();
	void sendRoomBoxes();
	void flamebarrelflicker();

	void sendPlayerBox();
	void sendWorldBoxes();

	//Show Menus
	void showTeamSelect();
	void showClassSelect();
	void playerIconsSet();

	//Set menu text
	void setHUDText(int menuID);

	//Menu activations
	bool startTeamSelect; //temp
	bool renderMenu;
	bool menuIpKeyListener;
	bool menuNameKeyListener;
	bool menuSensKeyListener;
	int nameNrOfKeys;
	int ipNrOfKeys;
	int senseNrOfKeys;
	bool escActive;
	bool inGameSettings;

	bool shitBool = false;
	bool justAFrameCounterActivated = false;
	int justAFrameCounter = 0;
	bool warmUpBannerActive;

	//time
	int oldTime;

	//Menu input
	void menuIpKeyInputUpdate();
	void menuNameKeyInputUpdate();
	void menuSensitivityKeyInputUpdate();
	const int validKeyboardInputs[VALIDKEYSIP] = { GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9, GLFW_KEY_0, GLFW_KEY_PERIOD };
	const int validKeyboardInputs2[VALIDKEYSNAME] = { GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_T, GLFW_KEY_Y, GLFW_KEY_U, GLFW_KEY_I, GLFW_KEY_O, GLFW_KEY_P,
		GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L,
		GLFW_KEY_Z, GLFW_KEY_X, GLFW_KEY_C, GLFW_KEY_V, GLFW_KEY_B, GLFW_KEY_N, GLFW_KEY_M, GLFW_KEY_COMMA, GLFW_KEY_PERIOD, GLFW_KEY_SPACE};
	const int validKeyboardInputs3[VALIDKEYSENS] = { GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9, GLFW_KEY_0 };
	//GUD text output manager
	std::string hudTextOutPutManager(bool onlyOne, int first, int second = 0);

	//for take damage blur effect on player or spectated player
	int lastHP_blurreffect = 0;

	//Settings data
	float* soundSavedData;
	float* optionsSavedData;
	int optionsDataSize;
	float currentSense;

	int round;

	bool calcTimer;
	bool justSetFullScreenMainMenu;
	bool justSetFullScreenIngame;
	bool justSetFullScreen;

public:
	
	vec3 TEAMONECOLOR = vec3(1.0f, 0.5f, 0.0f);
	vec3 TEAMTWOCOLOR = vec3(0.0f, 0.9f, 0.0f);
	vec3 CLEANSENOVACOLOR = vec3(1.0f, 1.0f, 1.0f);

	void init();
	void update(float dt);

	bool windowVisible();

	void setfps(int fps);

	~Core();

	//temp
	bool showOrHide = false;
};

#endif