#include "UIManager.h"

UIManager::UIManager()
{
	fileNamesList = nullptr;
	menus = nullptr;
	nrOfFileNames = 0;
	nrOfMenus = 0;
	maxMenus = 5;
	currentMenu = -1;
}
UIManager::~UIManager() 
{
}

//Start menu
void UIManager::init(std::string fileName, Console console)
{
	this->console = &console;

	std::ifstream myfile;
	myfile.open(fileName);

	if (myfile.is_open())
	{
		int counter = 0;
		std::string inputString;

		//Number of menu files.
		getline(myfile, inputString);
		nrOfFileNames = std::stoi(inputString);

		fileNamesList = new std::string[nrOfFileNames];

		while (!myfile.eof()) //Looping through the file until there is nothing left to read.
		{
			//Get the next file name
			getline(myfile, inputString);

			//Store the name
			fileNamesList[counter] = inputString;
			counter++;
		}
	}
	else
		console.printMsg("Error: UIManager filenames file could not be opened.", "System" ,'S' );
	myfile.close();

	

	menus = new UI[maxMenus];
}

//Add and remove menus
bool UIManager::addMenu(int fileId) 
{
	bool result = false; 

	if (nrOfMenus < maxMenus)
	{
		if (fileId >= 0 && fileId < nrOfFileNames)
		{
			if (menus == nullptr)
				menus = new UI[maxMenus];

			result = menus[nrOfMenus].loadUI(fileNamesList[fileId]);

			nrOfMenus++;
		}
		//else
		//	std::cout << "Error: File name id out of range." << std::endl;
	}
	//else
	//	std::cout << "Error: Max menus opened already." << std::endl;

	currentMenu++;

	return result;
}
bool UIManager::removeMenu(int menuId) 
{
	bool result = false;

	if (nrOfMenus > 0)
	{
		if (menuId >= 0 && menuId < nrOfMenus)
		{
			menus[menuId].clean();

			for (int i = menuId; i < nrOfMenus-1; i++)
			{
				menus[i] = menus[i + 1];
				menus[i].changeMenuId();
			}

			nrOfMenus--;
			result = true;
		}
		//else
		//	std::cout << "Error: MenuId is out of range." << std::endl;
	}
	//else
	//	std::cout << "Error: No menus to remove." << std::endl;

	currentMenu--;

	return result;
}
void UIManager::removeAllMenus() 
{
	for (int i = 0; i < nrOfMenus; i++)
	{
		menus[i].clean();
	}
	delete[] menus;
	nrOfMenus = 0;
	currentMenu = -1;
	delete[] fileNamesList;
	menus = nullptr;
}

int UIManager::collisionCheck(glm::vec2 pos)
{
	return menus[currentMenu].mouseCollission(pos);
}

uiVertex* UIManager::returnPosAUv(int id)
{
	return menus[currentMenu].returnPosAUv(id);
}
int* UIManager::returnTextureList()
{
	return menus[currentMenu].returnTextureList();
}
int UIManager::returnObjCount()
{
	return menus[currentMenu].returnObjCount();
}

glm::mat4* UIManager::returnWorldMatrix(int id)
{
	return menus[currentMenu].returnWorldMatrix(id);
}

int UIManager::changeTex(int objId)
{
	return menus[currentMenu].changeTex(objId);
}