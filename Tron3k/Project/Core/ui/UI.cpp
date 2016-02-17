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

void UI::renderMenu()
{
	uiRender->ui_initRender();
	for (int i = 0; i < UiObjects.size(); i++)
		UiObjects[i]->renderQuad(i);


	for (int i = 0; i < textObjRenderId.size(); i++)
		UiObjects[textObjRenderId[i]]->renderText(i);
}

void UI::renderIngame()
{
	uiRender->ui_InGameRenderInit();
	for (int i = 0; i < UiObjects.size(); i++)
		UiObjects[i]->renderQuad(i);

	for (int i = 0; i < textObjRenderId.size(); i++)
		UiObjects[textObjRenderId[i]]->renderText(i);
}

void UI::init(std::string fileName, Console* console, IRenderPipeline* uiRender, std::vector<glm::vec2>* textureRes, int winX, int winY)
{
	this->textureRes = textureRes;
	this->uiRender = uiRender;
	this->console = console;

	for (int i = 0; i < 11; i++)
	{
		textIdList[i] = -1;
	}

	bool result = loadUI(fileName, winX, winY);
	if (!result)
		console->printMsg("Error: LoadUI in UI was unsucessfull","System",'S');
}

//Needs to be modified
bool UI::loadUI(std::string fileName, int winX, int winY)
{
	bool result = false;
	
	std::ifstream myfile;
	myfile.open(fileName);
	
	if (myfile.is_open())
	{
		//Variables for the while loop
		std::string inputString;
		std::string textId;
		int convertedResult = -1;
		float x = -1.0f, y = -1.0f, u = -1.0f, v = -1.0f;
		int textureId1 = -1, textureId2 = -1, scale = -1, counter = 0, uniqueKey = 0, classId = -1, hoverKey = 0, tmpCounter = 0, scalingBar;
		int* textureArray = nullptr;
		glm::vec3 offsetTextSize;

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
			if (classId == 0)
			{
				getline(myfile, inputString); //tex1
				textureId1 = std::stoi(inputString);
				getline(myfile, inputString); //tex2
				textureId2 = std::stoi(inputString);

				getline(myfile, inputString); //uniqueKey
				uniqueKey = std::stoi(inputString);

				getline(myfile, inputString); //hoverKey
				hoverKey = std::stoi(inputString);
			}
			else if (classId == 1)
			{
				getline(myfile, inputString);
				scalingBar = std::stoi(inputString);

				if (scalingBar > 0)
				{
					getline(myfile, inputString); //textId
					textId = inputString;
				}

				getline(myfile, inputString);
				tmpCounter = std::stoi(inputString);

				textureArray = new int[tmpCounter];
				for (int i = 0; i < tmpCounter; i++)
				{
					getline(myfile, inputString);
					textureArray[i] = std::stoi(inputString);
				}
			}
			else if (classId == 4)
			{
				getline(myfile, inputString); //texture1
				textureId1 = std::stoi(inputString);

				getline(myfile, inputString); //texture2
				textureId2 = std::stoi(inputString);

				getline(myfile, inputString); //textId
				textId = inputString;

				getline(myfile, inputString); //XOffSet
				offsetTextSize.x = std::stof(inputString);;
				getline(myfile, inputString); //YOffSet
				offsetTextSize.y = std::stof(inputString);;
				getline(myfile, inputString); //textSize
				offsetTextSize.z = std::stoi(inputString);

				getline(myfile, inputString); //uniqueKey
				uniqueKey = std::stoi(inputString);
			}

			if (classId == 0) //Button
			{
				UiObjects.push_back(new Button(xy, textureId1, textureId2, uniqueKey, hoverKey, uiRender, textureRes[0][textureId1], textureRes[0][textureId2]));
				textureIdList[counter] = textureId1;
				result = true;
				counter++;
			}
			else if (classId == 1) //StaticTextBox
			{
				if (textId == "abilityMeter")
					textIdList[9] = counter;
				else if(textId == "loseTicketsMeter")
					textIdList[10] = counter;

				UiObjects.push_back(new StaticTextureBoxes(xy, textureArray, tmpCounter, uiRender, textureRes[0]));
				textureIdList[counter] = textureArray[0];
				result = true;
				counter++;
				delete[] textureArray;
				textureArray = nullptr;
			}
			else if (classId == 3) //Slider
			{
				//UiObjects.push_back(Slider(xy, textureId1, textureId2, counter, counter+1, uiRender, textureRes[0][textureId1], textureRes[0][textureId2]));
				//textureIdList[counter] = textureId1;

				//counter++;
				//textureIdList[counter] = textureId2;
				//result = true;
				//counter++;
			}
			else if(classId == 4) //InputBox
			{
				UiObjects.push_back(new InputBox(xy, textureId1, textureId2, uniqueKey, uiRender, textureRes[0][textureId1], winX, winY, offsetTextSize));
				textObjRenderId.push_back(counter);
				textureIdList[counter] = textureId1;
				result = true;

				if (textId == "hpBar")
					textIdList[0] = counter;
				else if (textId == "ammo")
					textIdList[1] = counter;
				else if (textId == "ticketBar1")
					textIdList[2] = counter;
				else if (textId == "ticketBar2")
					textIdList[3] = counter;
				else if (textId == "wins1")
					textIdList[4] = counter;
				else if (textId == "wins2")
					textIdList[5] = counter;
				else if (textId == "time")
					textIdList[6] = counter;
				else if (textId == "ip")
					textIdList[7] = counter;
				else if (textId == "name")
					textIdList[8] = counter;
				counter++;
			}
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
	
	return hit;
}

void UI::mouseHover(glm::vec2 pos)
{
	for (int i = 0; i < nrOfObjects; i++)
		UiObjects[i]->hoverCheck(pos);
}


void UI::changeTex(int objId, int whichTex)
{
	if(objId < UiObjects.size())
		UiObjects[objId]->changeTexUsed(whichTex);
}

void UI::changeColorTeam(int whichTex)
{
	for (int i = 0; i < UiObjects.size(); i++)
		UiObjects[i]->changeTexUsed(whichTex);
}

void UI::hideWindow(int id)
{
	
}
void UI::showWindow(int id)
{
	
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

void UI::setWindowResolution(int winX, int winY)
{
	for (int i = 0; i < UiObjects.size(); i++)
	{
		UiObjects[i]->setWindowResolution(winX, winY);
	}
}

void UI::setText(std::string text, int id)
{
	if(textIdList[id] > -1)
		UiObjects[textIdList[id]]->setText(text);
}
std::string UI::getText(int id)
{
	if (textIdList[id] > -1)
		return UiObjects[textIdList[id]]->getText();
	return "";
}
void UI::removeLastInput(int id)
{
	if (textIdList[id] > -1)
		UiObjects[textIdList[id]]->removeLastInput();
}

void UI::clearText(int id)
{
	if (textIdList[id] > -1)
		UiObjects[textIdList[id]]->cleanText();
}

void UI::scaleBar(int id, float procentOfMax, bool fromRight)
{
	if (textIdList[id] > -1)
		UiObjects[textIdList[id]]->scaleBar(procentOfMax, fromRight);
}