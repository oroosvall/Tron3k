#pragma once
#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "UI.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

enum MainMenu
{
	StartMenu,
	Multiplayer,
	Connect,
};

enum InGameUI
{
	GUI,
	TeamSelect,
	ClassSelect
};

enum UISets
{
	Menu,
	InGame
};

enum TeamColors
{
	TeamOne,
	TeamTwo
};



class UIManager
{
private:
	
	//Sent in by Core
	Console* console;
	IRenderPipeline* renderPipe;

	//Menu attributes
	UI* menus;
	int nrOfMenus;
	int maxMenus;
	int* currentMenu;
	int nrOfCurretMenus;
	int* openedMenus;
	int nrOfOpenedMenus;

	int winX;
	int winY;

	//Since multiple menus needs to be rendered when the gui is opened
	bool guiOpened;

	bool firstMenuSet;

	//Lists of file names
	std::string* fileNamesListFirstGroup; //First set of menus(Those you can use before going ingame)
	std::string* fileNamesListSecondGroup; //Second set of menus(Those you can use while ingame)
	int nrOfFileNamesFirstGroup;
	int nrOfFileNamesSecondGroup;
	int currentGroup;

	//Render stuff
	std::vector<glm::vec2>* textureRes;
	std::vector<GLuint> uiTextureIds;
	std::vector<std::string> texturePaths;

	bool roleSet = false;

	int teamColor;
	bool doHoverCheckInGame;

	//	Functions
	void loadInTexture();

public:
	UIManager();
	~UIManager();

	void setRoleBool(bool roleSet) { this->roleSet = roleSet; };
	bool getRoleBool() { return roleSet; };

	void setRenderPtr(IRenderPipeline* ptr) 
	{ 
		renderPipe = ptr; 
	}

	void init(Console* console, int winX, int winY);
	
	void menuRender();
	void inGameRender();

	void setMenu(int menuId);
	void backToGui();
	void removeAllMenus();

	int collisionCheck(glm::vec2 pos);
	void hoverCheck(glm::vec2 pos);

	void changeTex(int objId, int whichTex);
	void changeColorTeam();

	bool LoadNextSet(int whichMenuGroup, int winX, int winY);

	void setOpenedGuiBool(bool guiBool);
	void setFirstMenuSet(bool set);

	void setWindowResolution(int winX, int winY);

	void setText(std::string text, int id);
	std::string getText(int id);
	void removeLastInput(int id);
	void clearText(int id);
	void scaleBar(int id, float procentOfMax, bool fromRight);

	void setTeamColor(int team);
	int getTeamColor();

	void setHoverCheckBool(bool checkForHover);
	bool getHoverCheckBool();

	int getNrOfCurretMenus();
	int getCurrentMenu();

	struct HUDvalues
	{
		int HP;
		int ammo;
		float specialMeter;
		int teamOneTokens;
		int teamTwoTokens;
		int maxTokens;
		int teamOneRoundWins;
		int teamTwoRoundWins;
		int time;
		int ticketLostTimer;
		int loseTicketPer;
		bool firstSecondEachRound;
	}HUD;
};

#endif