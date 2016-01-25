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
	if ( fileNamesList != nullptr )
		delete [] fileNamesList;
	if ( menus != nullptr )
		delete[] menus;
}

//Start menu
void UIManager::init(std::string fileName)
{
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
	//else
		//std::cout << "Error: UIManager file: " << filename << " could not be opened." << std::endl;
	myfile.close();

	menus = new UI[maxMenus];
}

//Add and remove menus
void UIManager::removeAllMenus() 
{
	for (int i = 0; i < nrOfMenus; i++)
	{
		menus[i].clean();
	}
	nrOfMenus = 0;
	currentMenu = -1;
	delete menus;
	menus = nullptr;
}

int UIManager::collisionCheck(glm::vec2 pos, int whichMenu)
{
	return menus[whichMenu].mouseCollission(pos);
}

int UIManager::changeTex(int objId)
{
	return menus[currentMenu].changeTex(objId);
}