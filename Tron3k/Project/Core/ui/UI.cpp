#include "UI.h"

UI::UI() 
{
	//Pointers
	textureIdList = nullptr;
	//Counters
	nrOfObjects = 0;
	nrOfObjectsToRender = 0;

	//id
	menuId = -1;
}
UI::~UI() 
{
	clean();
}

void UI::render(std::vector<GLuint> uiTextureIds)
{
	uiRender->disableDepthTest();
	for (int i = 0; i < UiObjects.size(); i++)
		UiObjects[i]->render(i);
	uiRender->enableDepthTest();
}

void UI::init(std::string fileName, Console* console, IRenderPipeline* uiRender, std::vector<glm::vec2>* textureRes)
{
	this->textureRes = textureRes;
	this->uiRender = uiRender;
	this->console = console;

	bool result = loadUI(fileName);
	if (!result)
		console->printMsg("Error: LoadUI in UI was unsucessfull","System",'S');
}

//Needs to be modified
bool UI::loadUI(std::string fileName) 
{
	bool result = false;
	
	std::ifstream myfile;
	myfile.open(fileName);
	
	if (myfile.is_open())
	{
		//Variables for the while loop
		std::string inputString;
		int convertedResult = -1;
		float x = -1.0f, y = -1.0f, u = -1.0f, v = -1.0f;
		int textureId1 = -1, textureId2 = -1, scale = -1, counter = 0, uniqueKey = 0, classId = -1, hoverKey = 0, tmpCounter = 0;;
		int* textureArray = nullptr;

		//Number of objects
		getline(myfile, inputString);
		nrOfObjects = std::stoi(inputString); //Converting
	
		//Which menu
		getline(myfile, inputString);
		menuId = std::stoi(inputString);
	
		//Allocate memory
		textureIdList = new int[nrOfObjects];
	
		while (!myfile.eof()) //Looping through the file until there is nothing left to read.
		{
			std::string temp;
			glm::vec2 xy;
			std::stringstream ss("");
	
			//Positions and UVs
			//stoi doesn't work with decimals
			getline(myfile, inputString); //X, Y
			ss = std::stringstream(inputString);
			ss >> temp;
			xy.x = std::stof(temp);
			ss >> temp;
			xy.y = std::stof(temp);
	
			//Which class
			getline(myfile, inputString);
			classId = std::stoi(inputString);
			if (classId == 1)
			{
				getline(myfile, inputString);
				tmpCounter = std::stoi(inputString);

				textureArray = new int[tmpCounter];
				for (int i = 0; i < tmpCounter; i++)
				{
					getline(myfile, inputString);
					textureArray[i] = std::stoi(inputString);
				}
			}
			else
			{
				getline(myfile, inputString); //tex1
				textureId1 = std::stoi(inputString);
				getline(myfile, inputString); //tex2
				textureId2 = std::stoi(inputString);
			}

			getline(myfile, inputString); //uniqueKey
			uniqueKey = std::stoi(inputString);

			getline(myfile, inputString); //hoverKey
			hoverKey = std::stoi(inputString);

			if (classId == 0) //Button
			{
				UiObjects.push_back(new Button(xy, textureId1, textureId2, uniqueKey, hoverKey, uiRender, textureRes[0][textureId1], textureRes[0][textureId2]));
				textureIdList[counter] = textureId1;
				result = true;
				counter++;
			}
			else if (classId == 1) //StaticTextBox
			{
				UiObjects.push_back(new StaticTextureBoxes(xy, textureArray, tmpCounter, uiRender, textureRes[0]));
				textureIdList[counter] = textureArray[0];
				result = true;
				counter++;
				delete[] textureArray;
				textureArray = nullptr;
			}
			else if (classId == 3) //Slider
			{
				//UiObjects.push_back(Slider(xy, textureId1, textureId2, uniqueKey, counter, counter+1, uiRender, textureRes[0][textureId1], textureRes[0][textureId2]));
				//textureIdList[counter] = textureId1;

				//counter++;
				//textureIdList[counter] = textureId2;
				//result = true;
				//counter++;
			}
			//else if(classId == 2) {} //DynamicText
			//else if(classId == 4) {} //Input window
		}
	}
	else
		console->printMsg("Error: File could not be opened", "System", 'S');
	
	myfile.close();
	
	return result;
}

void UI::clean()
{
	for (int i = 0; i < UiObjects.size(); i++)
		if (UiObjects[i] != nullptr)
		{
			delete UiObjects[i];
			UiObjects[i] = nullptr;
		}
	if (textureIdList != nullptr)
		delete[] textureIdList;


	textureIdList = nullptr;
	console = nullptr;

	nrOfObjects = 0;
	nrOfObjectsToRender = 0;
	menuId = 0;
}

glm::vec2 UI::fileCoordToScreenSpace(glm::vec2 pos)
{
	glm::vec2 newVec2;

	newVec2.x = pos.x * 2 - 1;
	newVec2.y = pos.y * 2 - 1;

	return newVec2;
}


int UI::mouseCollission(glm::vec2 pos) 
{
	int hit = -1;
	int result = -1;
	
	for (int i = 0; i < nrOfObjects && hit == -1; i++)
	{
		hit = UiObjects[i]->checkCollision(pos);
	}
	
	//if (hit != -1)
	//	result = collisionEvent(hit);
	
	return hit;
}

void UI::mouseHover(glm::vec2 pos)
{
	for (int i = 0; i < nrOfObjects; i++)
		UiObjects[i]->hoverCheck(pos);
}

int UI::collisionEvent(int UniqueKey) //Every button in every menu have a unique key
{
	int result = -1;

	//Texture change is suppose to be changed to tex 1
	switch (UniqueKey)
	{
	case(4) : //IP
		//Här ska det sättas så man kan skriva in en IP
		break;
	//case(0) : //Roam 0
	//case(1) : //Play 1
	//case(2) : //Settings 2
	//case(3) : //Exit 3
	//case(5) : //Connect 5
	//case(6) : //Back 6
	default: //Everthing that needs to go back to UIManager is defaulted with returning their unique key.
		result = UniqueKey;
		break;
	}

	return result;
}


void UI::changeTex(int objId, int whichTex)
{
	UiObjects[objId]->changeTexUsed(whichTex);
}

//Empty
void hideWindow()
{

}

void UI::setWorldMatrix(float x, float y, int objId)
{
//	UiObjects[objId].setWorldMatrix(x, y);
}

//Empty
void UI::removeMenu() 
{

}

void UI::setTextureId(std::vector<GLuint> uiTextureIds)
{
	for (int i = 0; i < UiObjects.size(); i++)
	{
		UiObjects[i]->setTexture(uiTextureIds);
	}
}