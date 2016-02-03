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

	guiOpened = false;
	firstMenuSet = false;
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
void UIManager::init(Console* console)
{
	std::string fileNameFirstGroup = "GameFiles/UIFiles/MenuFileNames.txt";
	std::string fileNameSecondGroup = "GameFiles/UIFiles/InGameFileNames.txt";
	this->console = console;

	//Texture Paths
	//Main menu
	texturePaths.push_back("GameFiles/Textures/UITextures/main_menu_background.png"); //0
	texturePaths.push_back("GameFiles/Textures/UITextures/testmap.png"); //1
	texturePaths.push_back("GameFiles/Textures/UITextures/multiplayer.png"); //2
	texturePaths.push_back("GameFiles/Textures/UITextures/settings.png"); //3
	texturePaths.push_back("GameFiles/Textures/UITextures/exit.png"); //4

	//Multiplayer
	//Background  //5
	//Client  //6
	//Server  //7

	//Connect
	texturePaths.push_back("GameFiles/Textures/UITextures/connect_to_server_background.png"); //5
	texturePaths.push_back("GameFiles/Textures/UITextures/connect_to_server_connect.png"); //6
	texturePaths.push_back("GameFiles/Textures/UITextures/connect_to_server_back.png"); //7

	//Team select
	texturePaths.push_back("GameFiles/Textures/UITextures/team_selection_background.png"); //8
	texturePaths.push_back("GameFiles/Textures/UITextures/team_selection_alpha.png"); //9
	texturePaths.push_back("GameFiles/Textures/UITextures/team_selection_beta.png"); //10

	//Class select
	texturePaths.push_back("GameFiles/Textures/UITextures/class_selection_background.png"); //11
	texturePaths.push_back("GameFiles/Textures/UITextures/class_selection_trapper.png"); //12
	texturePaths.push_back("GameFiles/Textures/UITextures/class_selection_stalker.png"); //13
	texturePaths.push_back("GameFiles/Textures/UITextures/class_selection_punisher.png"); //14

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

	LoadNextSet(0); //Load the first set of menus.
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
	renderPipe->ui_initRender();
	
	renderPipe->disableDepthTest();
	menus[currentMenu[0]].render(uiTextureIds);
	renderPipe->enableDepthTest();
}

void UIManager::inGameRender()
{
	renderPipe->ui_InGameRenderInit();

	renderPipe->disableDepthTest();
	for (int i = 0; i < nrOfCurretMenus; i++)
		menus[currentMenu[i]].render(uiTextureIds);
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
			currentMenu[nrOfCurretMenus] = menuId;
			nrOfCurretMenus++;
		}
		else if(firstMenuSet)//Changing menu
		{
			openedMenus[nrOfOpenedMenus] = currentMenu[0];
			currentMenu[0] = menuId;
			nrOfOpenedMenus++;
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
				nrOfOpenedMenus--;
				currentMenu[0] = openedMenus[nrOfOpenedMenus];
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
	return menus[currentMenu[nrOfCurretMenus - 1]].mouseCollission(pos);
}
void UIManager::hoverCheck(glm::vec2 pos)
{
	menus[currentMenu[nrOfCurretMenus - 1]].mouseHover(pos);
}


void UIManager::changeTex(int objId, int whichTex)
{
	menus[currentMenu[nrOfCurretMenus - 1]].changeTex(objId, whichTex);
}

bool UIManager::LoadNextSet(int whichMenuGroup)
{
	removeAllMenus();

	if (currentGroup == 0)
		currentGroup = 1;
	else
		currentGroup = 0;

	//Menu init has to take the GLuint texture id!

	switch (currentGroup)
	{
	case 0: //First Group
	{
		menus = new UI[nrOfFileNamesFirstGroup];
		openedMenus = new int[nrOfFileNamesFirstGroup];
		currentMenu = new int[nrOfFileNamesFirstGroup];
		for (int i = 0; i < nrOfFileNamesFirstGroup; i++)
		{
			menus[i].init(fileNamesListFirstGroup[i], console, renderPipe, textureRes);
			menus[i].setTextureId(uiTextureIds);
			nrOfMenus++;
		}
		break;
	}
	case 1: //Second Group
	{
		menus = new UI[nrOfFileNamesSecondGroup];
		openedMenus = new int[nrOfFileNamesFirstGroup];
		currentMenu = new int[nrOfFileNamesFirstGroup];
		for (int i = 0; i < nrOfFileNamesSecondGroup; i++)
		{
			menus[i].init(fileNamesListSecondGroup[i], console, renderPipe, textureRes);
			menus[i].setTextureId(uiTextureIds);
			nrOfMenus++;
		}
		break;
	}
	default:
		break;
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