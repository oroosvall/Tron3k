#include "UIManager.h"

UIManager::UIManager()
{
	fileNamesListFirstGroup = nullptr;
	fileNamesListSecondGroup = nullptr;
	menus = nullptr;
	openedMenus = nullptr;
	currentMenu = -1;
	nrOfFileNamesFirstGroup = 0;
	nrOfFileNamesSecondGroup = 0;
	nrOfMenus = 0;
	maxMenus = 5;
	textureRes = new std::vector<glm::vec2>;
	nrOfOpenedMenus = 0;

	hideAbleFileName = "GameFiles/UIFiles/HideAble.txt";
	hideAbleMenuActive = false;

	teamColor = 0;

	firstMenu = false;
	doHoverCheckInGame = false;

	HUD.HP = 1;
	HUD.ammo = 0;
	HUD.specialMeter = 0.0f;
	HUD.teamOneScore = 0;
	HUD.teamTwoScore = 0;
	HUD.teamOneTokens = 0;
	HUD.teamTwoTokens = 0;
	HUD.maxTokens = 0;
	HUD.time = 0;
	HUD.ticketLostTimer = 0;
	HUD.loseTicketPer = 1;
	HUD.firstSecondEachRound = true;
	HUD.bannerCounter = 5;
	HUDTime.moveTokenReducer1 = false;
	HUDTime.moveTokenReducer2 = false;
	HUDTime.movePointAdder1 = false;
	HUDTime.movePointAdder2 = false;
}
UIManager::~UIManager() 
{
	hideAble.clean();
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
	HUD.bannerMaxTime.clear();
	
	HUDTime.wmIdListTicket1.clear();
	HUDTime.wmIdListTicket2.clear();
	HUDTime.wmIdListScore1.clear();
	HUDTime.wmIdListScore2.clear();
	HUDTime.counterListTicket1.clear();
	HUDTime.counterListTicket2.clear();
	HUDTime.counterListScore1.clear();
	HUDTime.counterListScore2.clear();


	optionsSaved = nullptr;
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
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/trapper.png"); //12
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/stalker.png"); //13
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/punisher.png"); //14

	//Ip input
	texturePaths.push_back("GameFiles/Textures/UITextures/Connect/ip_field.png"); //15
	//Port input
	texturePaths.push_back("GameFiles/Textures/UITextures/Connect/name_field.png"); //16

	//Title
	texturePaths.push_back("GameFiles/Textures/UITextures/title_new.png"); //17

	//Server
	texturePaths.push_back("GameFiles/Textures/UITextures/server.png"); //18
	
	//Multiplayer
	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/client_server_background.png"); //19 background
	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/client_button.png"); //20 client
	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/server_button.png"); //21 server

	//Hovers
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/multiplayer_hover.png"); //22
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/settings_hover.png"); //23
	texturePaths.push_back("GameFiles/Textures/UITextures/MainMenu/exit_hover.png"); //24

	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/client_button_hover.png"); //25
	texturePaths.push_back("GameFiles/Textures/UITextures/ClientServer/server_button_hover.png"); //26

	texturePaths.push_back("GameFiles/Textures/UITextures/Connect/connect_to_server_connect_hover.png"); //27
	texturePaths.push_back("GameFiles/Textures/UITextures/connect_to_server_back_hover.png"); //28

	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_alpha_hover.png"); //29
	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_beta_hover.png"); //30

	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/trapper_selected.png"); //31
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/stalker_selected.png"); //32
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/punisher_selected.png"); //33

	//Temp
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/banner_victory.png"); //34
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/banner_defeat.png"); //35
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/banner_objective_docks.png"); //36
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/banner_objective_fishmarket.png"); //37
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/banner_defend_the_objective.png"); //38
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/banner_final_assult.png"); //39
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/tickloss_1_green.png"); //40
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/pointgain_1_green.png"); //41
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/pointgain_2_green.png"); //42
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/pointgain_3_green.png"); //43
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/banner_end_of_round.png"); //44

	//New HUD
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/bot_background.png"); //45
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/hp_bar_orange.png"); //46
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/hp_bar_green.png"); //47
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/special_bar.png"); //48
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/wep_frame_orange.png"); //49
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/wep_frame_green.png"); //50
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/consumable_frame_orange.png"); //51
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/consumable_frame_green.png"); //52
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/timer_bar.png"); //53

	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/topframes_left.png"); //54 tickets team 1
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/topframes_center.png"); //55 timer
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/topframes_right.png"); //56 tickets team 2

	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/topframes_left_points.png"); //57 rounds team 1
	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/topframes_right_points.png"); //58 rounds team 1

	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/tickloss_2_green.png"); //59
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/tickloss_3_green.png"); //60

	texturePaths.push_back("GameFiles/Textures/UITextures/PlainHUD/NewVersion/countdown_background.png"); //61

	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/pointgain_1_orange.png"); //62
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/pointgain_2_orange.png"); //63
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/pointgain_3_orange.png"); //64

	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/tickloss_1_orange.png"); //65
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/tickloss_2_orange.png"); //66
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/tickloss_3_orange.png"); //67

	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/manipulator.png"); //68
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/manipulator_selected.png"); //69

	texturePaths.push_back("GameFiles/Textures/UITextures/r.png"); //70
	texturePaths.push_back("GameFiles/Textures/UITextures/Banners/banner_warmup.png"); //71
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_select_chooseYourClass.png"); //72
	texturePaths.push_back("GameFiles/Textures/UITextures/o.png"); //73
	texturePaths.push_back("GameFiles/Textures/UITextures/Settings/settings_background.png"); //74
	texturePaths.push_back("GameFiles/Textures/UITextures/Settings/settings_slider.png"); //75
	texturePaths.push_back("GameFiles/Textures/UITextures/Settings/settings_marker.png"); //76
	texturePaths.push_back("GameFiles/Textures/UITextures/Settings/settings_checkbox_unchecked.png"); //77
	texturePaths.push_back("GameFiles/Textures/UITextures/Settings/settings_checkbox_checked.png"); //78

	texturePaths.push_back("GameFiles/Textures/UITextures/EscWindow/pause_menu_background.png"); //79
	texturePaths.push_back("GameFiles/Textures/UITextures/EscWindow/pause_menu_quit_0.png"); //80
	texturePaths.push_back("GameFiles/Textures/UITextures/EscWindow/pause_menu_quit_1.png"); //81
	texturePaths.push_back("GameFiles/Textures/UITextures/EscWindow/pause_menu_resumegame_0.png"); //82
	texturePaths.push_back("GameFiles/Textures/UITextures/EscWindow/pause_menu_resumegame_1.png"); //83
	texturePaths.push_back("GameFiles/Textures/UITextures/EscWindow/pause_menu_settings_0.png"); //84
	texturePaths.push_back("GameFiles/Textures/UITextures/EscWindow/pause_menu_settings_1.png"); //85

	texturePaths.push_back("GameFiles/Textures/UITextures/Settings/settings_back.png"); //86
	texturePaths.push_back("GameFiles/Textures/UITextures/Settings/settings_back_hover.png"); //87
	texturePaths.push_back("GameFiles/Textures/UITextures/Settings/settings_confirmed.png"); //88
	texturePaths.push_back("GameFiles/Textures/UITextures/Settings/settings_confirmed_hover.png"); //89

	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/cluster.png"); //90 Consumable
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/dart.png"); //91 Consumable
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/thermite.png"); //92 Consumable
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/vortex.png"); //93 Consumable
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/discgun.png"); //94 Weaponds
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/tail.png"); //95
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/plasmacannon.png"); //96
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/batteryshots.png"); //97
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/shotgun.png"); //98
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/nadelauncher.png"); //99
	texturePaths.push_back("GameFiles/Textures/UITextures/Icons/pulserifle.png"); //100

	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_spectate.png"); //101
	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_spectate_hover.png"); //102
	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_cancel.png"); //103
	texturePaths.push_back("GameFiles/Textures/UITextures/TeamSelect/team_selection_cancel_hover.png"); //104

	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_select_round_1.png"); //105
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_select_round_2.png"); //106
	texturePaths.push_back("GameFiles/Textures/UITextures/ClassSelect/class_select_round_3.png"); //107

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

	currentGroup = -1;
	this->winX = winX;
	this->winY = winY;

	firstMenu = false;
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
	if(currentMenu > -1)
		menus[currentMenu].renderMenu();
}

void UIManager::inGameRender()
{
	renderPipe->disableDepthTest();
	//hideAble.renderIngame();

	for (int i = 0; i < nrOfOpenedMenus; i++)
		menus[openedMenus[i]].renderIngame();
	renderPipe->enableDepthTest();
}

//Add and remove menus
void UIManager::setMenu(int menuId)
{
	if (menuId > -1 && menuId < nrOfMenus)
	{
		if (!firstMenu)
		{
			firstMenu = true;
			nrOfOpenedMenus = 0;
		}
		if (nrOfOpenedMenus < maxMenus)
		{
			if (currentMenu == InGameUI::ClassSelect)
				menus[currentMenu].resetAllObjsTexture();

			openedMenus[nrOfOpenedMenus] = menuId;
			currentMenu = menuId;
			nrOfOpenedMenus++;

			if (currentMenu == MainMenu::Settings && currentGroup == 0)
				menus[currentMenu].setOptionsSaved(optionsSaved);
		}
	}
	else if (menuId == InGameUI::RemoveMenu)
	{
		if (nrOfOpenedMenus > -1 && openedMenus != nullptr)
		{
			if (openedMenus[nrOfOpenedMenus - 1] != InGameUI::GUI)
			{
				if (currentMenu == InGameUI::ClassSelect)
					menus[currentMenu].resetAllObjsTexture();

				nrOfOpenedMenus--;
				openedMenus[nrOfOpenedMenus] = -1;
				if (nrOfOpenedMenus > 0)
					currentMenu = openedMenus[nrOfOpenedMenus - 1];
				else
					currentMenu = -1;
			}
			else
			{
				//console->printMsg("Error: Function setMenu in UIManager, Someone is trying to use RemoveMenu when nrOfOpenedMenus has a value of 1 or lower .", "System", 'S');
			}
				
		}
	}
	else if (menuId == MainMenu::Back)
	{
		if (currentMenu == MainMenu::Connect)
		{
			menus[currentMenu].clearText(scaleAndText::IP);
			menus[currentMenu].clearText(scaleAndText::Name);
		}
		nrOfOpenedMenus--;
		if (nrOfOpenedMenus > 0)
			currentMenu = openedMenus[nrOfOpenedMenus - 1];
		else
			currentMenu = -1;
	}
	else
	{
		//console->printMsg("Error: Function setMenu in UIManager, menuId is invalid", "System", 'S');
	}
		
}
void UIManager::removeAllMenus()
{
	nrOfMenus = 0;
	hideAble.clean();
	if (openedMenus != nullptr)
		delete[] openedMenus;
	if(menus != nullptr)
		delete[] menus;
	menus = nullptr;
	openedMenus = nullptr;
	
	currentMenu = 0;
	nrOfOpenedMenus = 0;
	hideAbleMenuActive = false;
	HUDTime.moveTokenReducer1 = false;
	HUDTime.moveTokenReducer2 = false;
	HUDTime.movePointAdder1 = false;
	HUDTime.movePointAdder2 = false;
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
			currentMenu = -1;
			menus = new UI[nrOfFileNamesFirstGroup];
			openedMenus = new int[nrOfFileNamesFirstGroup];
			for (int i = 0; i < nrOfFileNamesFirstGroup; i++)
			{
				menus[i].init(fileNamesListFirstGroup[i], console, renderPipe, textureRes, winX, winY);
				menus[i].setTextureId(uiTextureIds);
				nrOfMenus++;
			}
			hideAbleMenuActive = false;
			break;
		}
		case 1: //Second Group
		{
			currentGroup = 1;
			currentMenu = -1;
			menus = new UI[nrOfFileNamesSecondGroup];
			openedMenus = new int[nrOfFileNamesSecondGroup];
			for (int i = 0; i < nrOfFileNamesSecondGroup; i++)
			{
				if (fileNamesListSecondGroup[i] != hideAbleFileName)
				{
					menus[i].init(fileNamesListSecondGroup[i], console, renderPipe, textureRes, winX, winY);
					menus[i].setTextureId(uiTextureIds);
					nrOfMenus++;
				}
				else
				{
					hideAble.init(fileNamesListSecondGroup[i], console, renderPipe, textureRes, winX, winY);
					hideAble.setTextureId(uiTextureIds);
					hideAbleMenuActive = true;
				}
			}
			break;
		}
		default:
			break;
		}
	}
	else
	{
		//console->printMsg("Abit meaningless load the same set of menus that you already use...", "System", 'S');
	}
		
	nrOfOpenedMenus = 0;

	return true;
}
void UIManager::setFirstMenuSet(bool set)
{
	firstMenu = set;
}


int UIManager::collisionCheck(glm::vec2 pos, float &newSoundProcent)
{
	if (nrOfOpenedMenus > 0)
		if (currentMenu > -1)
			return menus[currentMenu].mouseCollission(pos, newSoundProcent);
		else
		{
			//console->printMsg("Error: Function collisionCheck in UIManager, currentMenu has a value lower then 0", "System", 'S');
		}
			
	else
	{
		//console->printMsg("Error: Function collisionCheck in UIManager, nrOfOpenedMenus has a value of 0 or lower", "System", 'S');
	}
		
	return -1;
}
void UIManager::hoverCheck(glm::vec2 pos)
{
	if (nrOfOpenedMenus > 0)
		if (currentMenu > -1)
			menus[currentMenu].mouseHover(pos);
}


void UIManager::changeTex(int objId, int whichTex, int menu)
{
	if (nrOfOpenedMenus > 0)
		if (menu > -1)
			menus[menu].changeTex(objId, whichTex);
		else if(currentMenu > -1)
			menus[currentMenu].changeTex(objId, whichTex);
		else
		{
			//console->printMsg("Error: Function changeTex in UIManager, currentMenu has a value lower than 0", "System", 'S');
		}
			
	else
	{
		//console->printMsg("Error: Function changeTex in UIManager, nrOfOpenedMenus has a value of 0 or lower", "System", 'S');
	}
		
}

void UIManager::scaleAndTextChangeTexture(int objId, int whichTex, int menu)
{
	if (nrOfOpenedMenus > 0)
		if (menu > -1)
			menus[menu].scaleAndTextChangeTexture(objId, whichTex);
		else if(currentMenu > -1)
			menus[currentMenu].scaleAndTextChangeTexture(objId, whichTex);
		else
		{
			//console->printMsg("Error: Function changeTex in UIManager, currentMenu has a value lower than 0", "System", 'S');
		}
			
	else
	{
		//console->printMsg("Error: Function changeTex in UIManager, nrOfOpenedMenus has a value of 0 or lower", "System", 'S');
	}
		
}

void UIManager::changeColorTeam()
{
	if (currentGroup == 1)
		menus[InGameUI::GUI].changeColorTeam(teamColor);
}

void UIManager::setWindowResolution(int winX, int winY)
{
	this->winX = winX;
	this->winY = winY;

	if (nrOfOpenedMenus > 0)
		if (currentMenu > -1)
			menus[currentMenu].setWindowResolution(winX, winY);
		else
		{
			//console->printMsg("Error: Function setWindowResolution in UIManager, currentMenu has a value lower then 0", "System", 'S');
		}
			
	else
	{
	//	console->printMsg("Error: Function setWindowResolution in UIManager, nrOfOpenedMenus has a value of 0 or lower", "System", 'S');
	}
		
}

void UIManager::setText(std::string text, int id, int menu)
{
	if (nrOfOpenedMenus > 0)
		if(menu > -1)
			menus[menu].setText(text, id);
		else if(currentMenu > -1)
			menus[currentMenu].setText(text, id);
		else
		{
			//console->printMsg("Error: Function setText in UIManager, currentMenu has a value lower then 0", "System", 'S');
		}
			
	else
	{
		//console->printMsg("Error: Function setText in UIManager, nrOfOpenedMenus has a value of 0 or lower", "System", 'S');
	}
		
}

std::string UIManager::getText(int id, int menu)
{
	if (menu > -1)
		return menus[menu].getText(id);
	else if (currentMenu > -1)
		return menus[currentMenu].getText(id);
	return "";
}
void UIManager::removeLastInput(int id, int menu)
{
	if (nrOfOpenedMenus > 0)
		if (menu > -1)
			menus[menu].removeLastInput(id);
		else if(currentMenu > -1)
			menus[currentMenu].clearText(id);
		else
		{
			//console->printMsg("Error: Function removeLastInput in UIManager, currentMenu has a value lower then 0", "System", 'S');
		}
			
	else
	{
		//console->printMsg("Error: Function removeLastInput in UIManager, nrOfOpenedMenus has a value of 0 or lower", "System", 'S');
	}
		
}
void UIManager::clearText(int id, int menu)
{
	if (nrOfOpenedMenus > 0)
		if (menu > -1)
			menus[menu].clearText(id);
		else if(currentMenu > -1)
			menus[currentMenu].clearText(id);
		else
		{
			//console->printMsg("Error: Function clearText in UIManager, currentMenu has a value lower than 0", "System", 'S');
		}
			
	else
	{
		//console->printMsg("Error: Function clearText in UIManager, nrOfOpenedMenus has a value of 0 or lower", "System", 'S');
	}
		
}

void UIManager::scaleBar(int id, float procentOfMax, bool fromRight, int menu)
{
	if(nrOfOpenedMenus  > 0)
		if(menu > -1)
			menus[menu].scaleBar(id, procentOfMax, fromRight);
		else if(currentMenu > -1)
			menus[currentMenu].scaleBar(id, procentOfMax, fromRight);
		else
		{
			//console->printMsg("Error: Function scaleBar in UIManager, currentMenu has a value lower then 0", "System", 'S');
		}
			
	else
	{
		//console->printMsg("Error: Function scaleBar in UIManager, nrOfOpenedMenus has a value of 0 or lower", "System", 'S');
	}
		
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

int UIManager::getNrOfOpenedMenus()
{
	return nrOfOpenedMenus;
}
int UIManager::getCurrentMenu()
{
	//int returnValue = -1;
	//if (nrOfOpenedMenus > 0)
	//	if (currentMenu > -1)
	//		returnValue = currentMenu;
	return currentMenu;
}

bool UIManager::isThereAMenuUp()
{
	bool returnValue = true;
	//if (currentMenu == InGameUI::GUI && nrOfOpenedMenus == 1)
	if(currentMenu < 1)
		returnValue = false;

	return returnValue;
}

void UIManager::setOptionsSaved(float* list)
{
	optionsSaved = list;
	if(nrOfMenus > MainMenu::Settings)
		menus[MainMenu::Settings].setOptionsSaved(optionsSaved);
}
float* UIManager::getOptionsSaved()
{
	if (currentMenu == MainMenu::Settings)
		if (nrOfMenus > MainMenu::Settings)
			optionsSaved = menus[MainMenu::Settings].getOptionsSaved();

	return optionsSaved;
}

//hideAble exclusive
void UIManager::renderHideAble()
{
	if (hideAbleMenuActive)
	{
		renderPipe->ui_InGameRenderInit();
		hideAble.renderHideable();
	}
}

void UIManager::hideOrShowHideAble(int id, bool show)
{
	if (hideAbleMenuActive)
	{
		if (!show)
			hideAble.hideObject(id);
		else
			hideAble.showObject(id);
	}
	else
	{
		//console->printMsg("Function hideOrShowHideAble in UIManager, Hideablemenu isn't active", "System", 'S');
	}
		
}
void UIManager::changeTextureHideAble(int id, int wmID, int textureId)
{
	if (hideAbleMenuActive)
		hideAble.changeHideAbleTexture(id, wmID, textureId);
	else
	{
		//console->printMsg("Function changeTextureHideAble in UIManager, Hideablemenu isn't active", "System", 'S');
	}
		
}

void UIManager::setHideableWorldMatrix(int id, int wmId, glm::vec2 xy)
{
	hideAble.setHideableWorldMatrix(id, wmId,  xy);
}
void UIManager::resetHidableWorldMatrix(int id, int wmId)
{
	hideAble.resetHidableWorldMatrix(id, wmId);
}

int UIManager::addNewWM(int id)
{
	return hideAble.addNewWM(id);
}
void UIManager::deleteOldestWM(int id)
{
	hideAble.deleteOldestWM(id);
}

void UIManager::stopRendering(int id, bool r, int menu)
{
	if (menu > -1)
		menus[menu].stopRendering(id, r);
	else if (currentMenu > -1)
		menus[currentMenu].stopRendering(id, r);
}