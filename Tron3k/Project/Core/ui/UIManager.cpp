#include "UIManager.h"

UIManager::UIManager()
{
	fileNamesListFirstGroup = nullptr;
	fileNamesListSecondGroup = nullptr;
	menus = nullptr;
	nrOfFileNamesFirstGroup = 0;
	nrOfFileNamesSecondGroup = 0;
	nrOfMenus = 0;
	maxMenus = 5;

	currentMenu = -1;
}
UIManager::~UIManager() 
{
	if (fileNamesListFirstGroup != nullptr )
		delete [] fileNamesListFirstGroup;
	if (fileNamesListSecondGroup != nullptr)
		delete[] fileNamesListSecondGroup;
	if ( menus != nullptr )
		delete[] menus;
	console = nullptr;

	glDeleteShader(shader);
}

//Start menu
void UIManager::init(std::string fileNameFirstGroup, std::string fileNameSecondGroup, Console* console)
{
	this->console = console;

	//Texture Paths
	texturePaths.push_back("../../../Debug/GameFiles/Textures/UITextures/grassTexture.png");
	texturePaths.push_back("../../../Debug/GameFiles/Textures/UITextures/secondTexture.png");
	texturePaths.push_back("../../../Debug/GameFiles/Textures/UITextures/thirdTexture.png");
	texturePaths.push_back("../../../Debug/GameFiles/Textures/UITextures/fourthTexture.png");
	texturePaths.push_back("../../../Debug/GameFiles/Textures/UITextures/fifthTexture.png");

	//Render stuff
	shader;
	uniformtextureLocation = glGetUniformLocation(shader, "textureSample");
	uniformWorldMat = glGetUniformLocation(shader, "WorldMatrix");
	
	//GLuint temp;
	//std::string shaderNamesDeffered[] = { "GameFiles/Shaders/uiShader_vs.glsl", "GameFiles/Shaders/uiShader_fs.glsl" };
	//GLenum shaderTypesDeffered[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	////CreateProgram(temp, shaderNamesDeffered, shaderTypesDeffered, 2);
	//if (temp != 0)
	//{
	//	shader = temp;
	//	temp = 0;
	//}

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

	currentGroup = 0;

	LoadNextSet(0); //Load the first set of menus.
	setMenu(0); //Set start menu as the current menu
}

void UIManager::loadInTexture()
{
	uiTextureIds.clear();
	texturePaths.clear();
	GLuint temp;

	//for (int i = 0; i < texturePaths.size(); i++)
	//{
	//	temp = loadTexture(texturePaths[i]);
	//	uiTextureIds.push_back(temp);
	//}
}

void UIManager::render()
{
	//glDisable(GL_DEPTH_TEST);

	menus[currentMenu].render(uiTextureIds);

	//glEnable(GL_DEPTH_TEST);
}


//Add and remove menus
void UIManager::setMenu(int menuId)
{
	if (menuId > -1 && menuId < nrOfMenus)
		currentMenu = menuId;
	else
		console->printMsg("Error: invalid menuId in function setMenu.", "System", 'S');
}
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

int UIManager::collisionCheck(glm::vec2 pos)
{
	return menus[currentMenu].mouseCollission(pos);
}

void UIManager::changeTex(int objId)
{
	menus[currentMenu].changeTex(objId);
}

bool UIManager::LoadNextSet(int whichMenuGroup)
{
	removeAllMenus();

	if (currentGroup == 0)
		currentGroup = 1;
	else
		currentGroup = 0;

	switch (currentGroup)
	{
	case 0: //First Group
	{
		menus = new UI[nrOfFileNamesFirstGroup];
		for (int i = 0; i < nrOfFileNamesFirstGroup; i++)
			menus[i].init(fileNamesListFirstGroup[i], uniformtextureLocation, uniformWorldMat, shader, console);
		break;
	}
	case 1: //Second Group
	{
		menus = new UI[nrOfFileNamesSecondGroup];
		for (int i = 0; i < nrOfFileNamesSecondGroup; i++)
			menus[i].init(fileNamesListSecondGroup[i], uniformtextureLocation, uniformWorldMat, shader, console);
		break;
	}
	default:
		break;
	}

	return true;
}