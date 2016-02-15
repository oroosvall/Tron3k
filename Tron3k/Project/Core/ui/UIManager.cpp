#include "UIManager.h"

UIManager::UIManager()
{
	fileNamesListFirstGroup = nullptr;
	fileNamesListSecondGroup = nullptr;
	menus = nullptr;
	openedMenus = nullptr;
	currentMenu = nullptr;
	nrOfFileNamesFirstGroup = 0;
	nrOfFileNamesSecondGroup = 0;
	nrOfMenus = 0;
	maxMenus = 5;
	textureRes = new std::vector<glm::vec2>;
	nrOfCurretMenus = 0;
	nrOfOpenedMenus = 0;

	teamColor = 0;

	guiOpened = false;
	firstMenuSet = false;
	doHoverCheckInGame = false;

	HUD.HP = 1;
	HUD.ammo = 0;
	HUD.specialMeter = 0.0f;
	HUD.teamOneRoundWins = 0;
	HUD.teamTwoRoundWins = 0;
	HUD.teamOneTokens = 0;
	HUD.teamTwoTokens = 0;
	HUD.maxTokens = 0;
	HUD.time = 0;
	HUD.ticketLostTimer = 0;
	HUD.loseTicketPer = 1;
	HUD.firstSecondEachRound = true;
}
UIManager::~UIManager() 
{
	if (renderPipe != nullptr)
	{
		renderPipe->ui_textureRelease(uiTextureIds);
	}
	if (fileNamesListFirstGroup != nullptr )
		delete [] fileNamesListFirstGroup;
	if (fileNamesListSecondGroup != nullptr)
		delete[] fileNamesListSecondGroup;
	if ( menus != nullptr )
		delete[] menus;
	console = nullptr;
	if (textureRes)
		delete textureRes;
	if (openedMenus != nullptr)
		delete[] openedMenus;
	if (currentMenu != nullptr)
		delete[] currentMenu;
}

//Start menu
void UIManager::init(Console* console, int winX, int winY)
{
	std::string fileNameFirstGroup = "GameFiles/UIFiles/MenuFileNames.txt";
	std::string fileNameSecondGroup = "GameFiles/UIFiles/InGameFileNames.txt";
	this->console = console;

	//Texture Paths
	//Main menu
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/main_menu_background.png"); //0
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/testmap.png"); //1
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/multiplayer.png"); //2
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/settings.png"); //3
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/exit.png"); //4

	//Connect
	texturePaths.push_back("GameFiles/Textures/UITextures/Connect/connect_to_server_background.png"); //5
	texturePaths.push_back("GameFiles/Textures/UITextures/Connect/connect_to_server_connect.png"); //6
	texturePaths.push_back("GameFiles/Textures/UITextures/connect_to_server_back.png"); //7

	//Team select
	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_background.png"); //8
	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_alpha.png"); //9
	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_beta.png"); //10

	//Class select
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_selection_background.png"); //11
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_selection_trapper.png"); //12
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_selection_stalker.png"); //13
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_selection_punisher.png"); //14

	//Ip input
	texturePaths.push_back("GameFiles/Textures/UITextures/Connect/ip_field.png"); //15
	//Port input
	texturePaths.push_back("GameFiles/Textures/UITextures/Connect/name_field.png"); //16

	//Title
	texturePaths.push_back("GameFiles/Textures/UITextures/title_new.png"); //17

	//Server
	texturePaths.push_back("GameFiles/Textures/UITextures/server.png"); //18

	//GUI TEAM 1
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_left_frame_squared.png"); //19  Ammo, hp and weapons
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_left_bar_hp.png"); //20 hp bar
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_left_bar_special.png"); //21 special bar

	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_right_frame.png"); //22 tickets
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_right_bar_0.png"); //23 ticket team 1

	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_top_bar.png"); //24 capture point timer
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_top_frame_0.png"); //25 meter
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_top_frame_1.png"); //26 rounds team 1
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_top_frame_2.png"); //27 timer
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Orange/plain_hud_top_frame_3.png"); //28 rounds team 2

	//GUI TEAM 2
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_left_frame_squared_green.png"); //29  Ammo, hp and weapons
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_left_bar_hp_green.png"); //30 hp bar
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_left_bar_special_green.png"); //31 special bar

	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_right_frame_green.png"); //32 tickets
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_right_bar_1_green.png"); //33 tickets team 2

	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_top_bar_green.png"); //34 capture point timer
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_top_frame_0_green.png"); //35 meter
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_top_frame_1_green.png"); //36 rounds team 1
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_top_frame_2_green.png"); //37 timer
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/Green/plain_hud_top_frame_3_green.png"); //38 rounds team 2

	//Multiplayer
	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/client_server_background.png"); //39 background
	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/client_button.png"); //40 client
	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/server_button.png"); //41 server

	//Hovers
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/multiplayer_hover.png"); //42
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/settings_hover.png"); //43
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/exit_hover.png"); //44

	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/client_button_hover.png"); //45
	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/server_button_hover.png"); //46

	texturePaths.push_back("GameFiles/Textures/UITextures/Connect/connect_to_server_connect_hover.png"); //47
	texturePaths.push_back("GameFiles/Textures/UITextures/connect_to_server_back_hover.png"); //48

	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_alpha_hover.png"); //49
	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_beta_hover.png"); //50

	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_selection_trapper_hover.png"); //51
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_selection_stalker_hover.png"); //52
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_selection_punisher_hover.png"); //53

	//Släng in allt detta i en fil och läs in ifrån den vid här och lägg det i temp namn vector som sedans cleanas upp.


	loadInTexture();

	//Open tthe first groups file
	std::ifstream myfile;
	myfile.open(fileNameFirstGroup);

	if (myfile.is_open())
	{
		int counter = 0;
		std::string inputString;

		//Number of menu files.
		getline(myfile, inputString);
		nrOfFileNamesFirstGroup = std::stoi(inputString);

		fileNamesListFirstGroup = new std::string[nrOfFileNamesFirstGroup];

		while (!myfile.eof()) //Looping through the file until there is nothing left to read.
		{
			//Get the next file name
			getline(myfile, inputString);

			//Store the name
			fileNamesListFirstGroup[counter] = inputString;
			counter++;
		}
	}
	else
		console->printMsg("Error: UIManager.init could not open the first groups file.", "System", 'S');
	myfile.close();

	//Open the second groups file
	myfile.open(fileNameSecondGroup);

	if (myfile.is_open())
	{
		int counter = 0;
		std::string inputString;

		//Number of menu files.
		getline(myfile, inputString);
		nrOfFileNamesSecondGroup = std::stoi(inputString);

		fileNamesListSecondGroup = new std::string[nrOfFileNamesSecondGroup];

		while (!myfile.eof()) //Looping through the file until there is nothing left to read.
		{
			//Get the next file name
			getline(myfile, inputString);

			//Store the name
			fileNamesListSecondGroup[counter] = inputString;
			counter++;
		}
	}
	else
		console->printMsg("Error: UIManager.init could not open the second groups file.", "System", 'S');
	myfile.close();

	currentGroup = 1;
	this->winX = winX;
	this->winY = winY;

	LoadNextSet(0, winX, winY); //Load the first set of menus.
	setMenu(0); //Set start menu as the current menu
}

void UIManager::loadInTexture()
{
	uiTextureIds.clear();
	textureRes->clear();
	GLuint temp;
	for (int i = 0; i < texturePaths.size(); i++)
	{
		temp = 0; 
		int xres, yres;
		renderPipe->ui_loadTexture(&temp, (char*)(texturePaths[i].c_str()), &xres, &yres);
		textureRes[0].push_back(glm::vec2());
		textureRes[0][i].x = xres;
		textureRes[0][i].y = yres;
		uiTextureIds.push_back(temp);
	}
	texturePaths.clear();
}

void UIManager::menuRender()
{
	menus[currentMenu[0]].renderMenu();
}

void UIManager::inGameRender()
{
	renderPipe->ui_InGameRenderInit();

	renderPipe->disableDepthTest();
	for (int i = 0; i < nrOfCurretMenus; i++)
		menus[currentMenu[i]].renderIngame();
	renderPipe->enableDepthTest();
}


//Add and remove menus
void UIManager::setMenu(int menuId)
{
	if (menuId > -1 && menuId < nrOfMenus)
	{
		if (guiOpened) //guiOpened is true when the gui needs to be rendered, since menus needs to be rendered together with the gui.
		{
			if (!firstMenuSet)//Changing menu
			{
				nrOfCurretMenus = 0;
				firstMenuSet = true;
			}
			if (nrOfCurretMenus > -1)
			{
				currentMenu[nrOfCurretMenus] = menuId;
				nrOfCurretMenus++;
			}
			else
				console->printMsg("Error: Class UIManager line 254, nrOfCurretMenus has a value below 0", "System", 'S');
		}
		else if(firstMenuSet)//Changing menu
		{
			if ((nrOfOpenedMenus && currentMenu[0]) > -1)
			{
				openedMenus[nrOfOpenedMenus] = currentMenu[0];
				currentMenu[0] = menuId;
				nrOfOpenedMenus++;
			}
			else
				console->printMsg("Error: Class UIManager line 264, nrOfOpenedMenus OR currentMenu[0] have value below 0", "System", 'S');
		}
		else
		{
			currentMenu[0] = menuId;
			nrOfCurretMenus = 1;
			firstMenuSet = true;
		}
	}
	else if (menuId == -1) //This is for going back to the last menu
	{
		if (guiOpened) //remove extra windows ingame for example continue/quit window(esc) window or score window(tab).
			nrOfCurretMenus--;
		else //Going back in the menu
		{
			if (nrOfOpenedMenus > 0)
			{
				if (currentMenu[0] == 2)
				{
					menus[currentMenu[0]].clearText(7);
					menus[currentMenu[0]].clearText(8);
				}
				nrOfOpenedMenus--;
				if(nrOfOpenedMenus > -1)
					currentMenu[0] = openedMenus[nrOfOpenedMenus];
				else
					console->printMsg("Error: Class UIManager line 294, nrOfOpenedMenus has a value below 0", "System", 'S');
			}
		}
	}
	else
		console->printMsg("Error: invalid menuId in function setMenu.", "System", 'S');
}
void UIManager::backToGui() //This is used when you go back to esc window from settings
{
	nrOfCurretMenus = 2;
	nrOfOpenedMenus = 2;
	currentMenu[0] = openedMenus[0];
	currentMenu[1] = openedMenus[1];

	setOpenedGuiBool(true);
}
void UIManager::removeAllMenus() 
{
	nrOfMenus = 0;
	if (openedMenus != nullptr)
		delete[] openedMenus;
	if (currentMenu != nullptr)
		delete[] currentMenu;
	if(menus != nullptr)
		delete[] menus;
	menus = nullptr;
	openedMenus = nullptr;
	currentMenu = nullptr;

	nrOfOpenedMenus = 0;
	nrOfCurretMenus = 0;
}

int UIManager::collisionCheck(glm::vec2 pos)
{
	if ((nrOfCurretMenus - 1) > -1)
		if (currentMenu[nrOfCurretMenus - 1] > -1)
			return menus[currentMenu[nrOfCurretMenus - 1]].mouseCollission(pos);
		else
			console->printMsg("Error: Class UIManager line 333, currentMenu[nrOfCurretMenus - 1] has a value below 0", "System", 'S');
	else
		console->printMsg("Error: Class UIManager line 332, (nrOfCurretMenus - 1) has a value below 0", "System", 'S');
	return -1;
}
void UIManager::hoverCheck(glm::vec2 pos)
{
	if ((nrOfCurretMenus - 1) > -1)
		if (currentMenu[nrOfCurretMenus - 1] > -1)
			menus[currentMenu[nrOfCurretMenus - 1]].mouseHover(pos);
		else
			console->printMsg("Error: Class UIManager line 344, currentMenu[nrOfCurretMenus - 1] has a value below 0", "System", 'S');
	else
		console->printMsg("Error: Class UIManager line 343, (nrOfCurretMenus - 1) has a value below 0", "System", 'S');
}


void UIManager::changeTex(int objId, int whichTex)
{
	if ((nrOfCurretMenus - 1) > -1)
		if (currentMenu[nrOfCurretMenus - 1] > -1)
			menus[currentMenu[nrOfCurretMenus - 1]].changeTex(objId, whichTex);
		else
			console->printMsg("Error: Class UIManager line 356, currentMenu[nrOfCurretMenus - 1] has a value below 0", "System", 'S');
	else
		console->printMsg("Error: Class UIManager line 355, (nrOfCurretMenus - 1) has a value below 0", "System", 'S');
}

void UIManager::changeColorTeam()
{
	if (currentGroup == 1)
		menus[0].changeColorTeam(teamColor);
}

bool UIManager::LoadNextSet(int whichMenuGroup, int winX, int winY)
{
	removeAllMenus();

	if (currentGroup != whichMenuGroup)
	{
		switch (whichMenuGroup)
		{
		case 0: //First Group
		{
			currentGroup = 0;
			menus = new UI[nrOfFileNamesFirstGroup];
			openedMenus = new int[nrOfFileNamesFirstGroup];
			currentMenu = new int[nrOfFileNamesFirstGroup];
			for (int i = 0; i < nrOfFileNamesFirstGroup; i++)
			{
				menus[i].init(fileNamesListFirstGroup[i], console, renderPipe, textureRes, winX, winY);
				menus[i].setTextureId(uiTextureIds);
				nrOfMenus++;
			}
			break;
		}
		case 1: //Second Group
		{
			currentGroup = 1;
			menus = new UI[nrOfFileNamesSecondGroup];
			openedMenus = new int[nrOfFileNamesFirstGroup];
			currentMenu = new int[nrOfFileNamesFirstGroup];
			for (int i = 0; i < nrOfFileNamesSecondGroup; i++)
			{
				menus[i].init(fileNamesListSecondGroup[i], console, renderPipe, textureRes, winX, winY);
				menus[i].setTextureId(uiTextureIds);
				nrOfMenus++;
			}
			break;
		}
		default:
			break;
		}
	}
	nrOfCurretMenus = 0;

	return true;
}

void UIManager::setOpenedGuiBool(bool guiBool)
{
	guiOpened = guiBool;
}

void UIManager::setFirstMenuSet(bool set)
{
	firstMenuSet = set;
}

void UIManager::setWindowResolution(int winX, int winY)
{
	this->winX = winX;
	this->winY = winY;

	if ((nrOfCurretMenus - 1) > -1)
		if (currentMenu[nrOfCurretMenus - 1] > -1)
			menus[currentMenu[nrOfCurretMenus - 1]].setWindowResolution(winX, winY);
		else
			console->printMsg("Error: Class UIManager line 431, currentMenu[nrOfCurretMenus - 1] has a value below 0", "System", 'S');
	else
		console->printMsg("Error: Class UIManager line 430, (nrOfCurretMenus - 1) has a value below 0", "System", 'S');
}

void UIManager::setText(std::string text, int id)
{
	if ((nrOfCurretMenus - 1) > -1)
		if(currentMenu[nrOfCurretMenus - 1] > -1)
			menus[currentMenu[nrOfCurretMenus - 1]].setText(text, id);
		else
			console->printMsg("Error: Class UIManager line 442, currentMenu[nrOfCurretMenus - 1] has a value below 0", "System", 'S');
	else
		console->printMsg("Error: Class UIManager line 441, (nrOfCurretMenus - 1) has a value below 0", "System", 'S');
}

std::string UIManager::getText(int id)
{
	if (currentMenu[nrOfCurretMenus - 1] > -1)
		return menus[currentMenu[nrOfCurretMenus - 1]].getText(id);
	return "";
}
void UIManager::removeLastInput(int id)
{
	if ((nrOfCurretMenus - 1) > -1)
		if (currentMenu[nrOfCurretMenus - 1] > -1)
			menus[currentMenu[nrOfCurretMenus - 1]].removeLastInput(id);
		else
			console->printMsg("Error: Class UIManager line 458, currentMenu[nrOfCurretMenus - 1] has a value below 0", "System", 'S');
	else
		console->printMsg("Error: Class UIManager line 463, (nrOfCurretMenus - 1) has a value below 0", "System", 'S');
}
void UIManager::clearText(int id)
{
	if ((nrOfCurretMenus - 1) > -1)
		if (currentMenu[nrOfCurretMenus - 1] > -1)
			menus[currentMenu[nrOfCurretMenus - 1]].clearText(id);
		else
			console->printMsg("Error: Class UIManager line 469, currentMenu[nrOfCurretMenus - 1] has a value below 0", "System", 'S');
	else
		console->printMsg("Error: Class UIManager line 468, (nrOfCurretMenus - 1) has a value below 0", "System", 'S');
}

void UIManager::scaleBar(int id, float procentOfMax, bool fromRight)
{
	if((nrOfCurretMenus - 1) > -1)
		if(currentMenu[nrOfCurretMenus - 1] > -1)
			menus[currentMenu[nrOfCurretMenus - 1]].scaleBar(id, procentOfMax, fromRight);
		else
			console->printMsg("Error: Class UIManager line 480, currentMenu[nrOfCurretMenus - 1] has a value below 0", "System", 'S');
	else
		console->printMsg("Error: Class UIManager line 479, (nrOfCurretMenus - 1) has a value below 0", "System", 'S');
}

void UIManager::setTeamColor(int team)
{
	if(team == 1 )
		teamColor = 0;
	else if(team == 2)
		teamColor = 1;
}
int UIManager::getTeamColor()
{
	return teamColor;
}

void UIManager::setHoverCheckBool(bool checkForHover)
{
	doHoverCheckInGame = checkForHover;
}
bool UIManager::getHoverCheckBool()
{
	return doHoverCheckInGame;
}

int UIManager::getNrOfCurretMenus()
{
	return nrOfCurretMenus;
}
int UIManager::getCurrentMenu()
{
	int returnValue = -1;
	if ((nrOfCurretMenus - 1) > -1)
		if (currentMenu[nrOfCurretMenus - 1] > -1)
			returnValue = currentMenu[nrOfCurretMenus - 1];
		else
			console->printMsg("Error: Class UIManager line 517, currentMenu[nrOfCurretMenus - 1] has a value below 0", "System", 'S');
	else
		console->printMsg("Error: Class UIManager line 516, (nrOfCurretMenus - 1) has a value below 0", "System", 'S');
	return returnValue;
}

bool UIManager::isThereAMenuUp()
{
	bool returnValue = true;
	if (currentGroup == 1)
		if (currentMenu[0] == 0 && nrOfCurretMenus == 1)
			returnValue = false;

	return returnValue;
}