#include "UI.h"

UI::UI() 
{
	buttons = nullptr;
	sliders = nullptr;
	staticText = nullptr;
	//dynamicTextBoxes = nullptr;
	//inputBoxes = nullptr;

	objIdList = nullptr;
	textureList = nullptr;

	nrOfbuttons = 0;
	nrOfsliders = 0;
	nrOfstaticText = 0;
	nrOfdynamicTextBoxes = 0;
	nrOfinputBoxes = 0;

	nrOfObjects = 0;
	menuId = 0;
}
UI::~UI() 
{
	clean();
}

void UI::clean()
{
	if (buttons != nullptr)
		delete[] buttons;
	if (sliders != nullptr)
		delete[] sliders;
	if (staticText != nullptr)
		delete[] staticText;
	//if (dynamicTextBoxes != nullptr)
	//	delete[] dynamicTextBoxes;
	//if (inputBoxes != nullptr)
	//	delete[] inputBoxes;
	if (objIdList != nullptr)
		delete[] objIdList;
	if (textureList != nullptr)
		delete[] textureList;

	buttons = nullptr;
	sliders = nullptr;
	staticText = nullptr;
	//dynamicTextBoxes = nullptr;
	//inputBoxes = nullptr;

	objIdList = nullptr;
	textureList = nullptr;

	nrOfObjects = 0;
	menuId = 0;
}

void UI::changeMenuId()
{
	menuId--;
}

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
		int textureId1 = -1, textureId2 = -1, scale = -1, counter = 0, uniqueKey = 0, counterButtons = 0, counterSliders = 0, counterStaticText = 0, counterDynamicBox = 0, counterInputBoxes = 0;

		//Number of objects
		getline(myfile, inputString);
		nrOfObjects = std::stoi(inputString); //Converting

		//Which menu
		getline(myfile, inputString);
		menuId = std::stoi(inputString);

		getline(myfile, inputString);
		nrOfbuttons = std::stoi(inputString);
		getline(myfile, inputString);
		nrOfsliders = std::stoi(inputString);
		getline(myfile, inputString);
		nrOfstaticText = std::stoi(inputString);
		getline(myfile, inputString);
		nrOfdynamicTextBoxes = std::stoi(inputString);
		getline(myfile, inputString);
		nrOfinputBoxes = std::stoi(inputString);

		if (nrOfbuttons > 0)
			buttons = new Button[nrOfbuttons];
		if (nrOfsliders > 0)
			sliders = new Slider[nrOfsliders];
		if (nrOfstaticText > 0)
			staticText = new StaticBox[nrOfstaticText];
		//if (nrOfdynamicTextBoxes > 0)
		//	dynamicTextBoxes = new DynamicTextBox[nrOfdynamicTextBoxes];
		//if (nrOfinputBoxes > 0)
		//	inputBoxes = new InputBox[nrOfinputBoxes];

		//Allocate memory
		objIdList = new int[nrOfObjects];
		textureList = new int[nrOfObjects];

		while (!myfile.eof()) //Looping through the file until there is nothing left to read.
		{
			getline(myfile, inputString); //Get the class key to see which class object that needs to be created.
			convertedResult = std::stoi(inputString);

			if (convertedResult == 0 || convertedResult == 1) //Button or Slider class
			{
				std::string temp;
				glm::vec2 xy[4];
				glm::vec2 uv[4];
				std::stringstream ss("");

				for (int i = 0; i < 4; i++) //stoi doesn't work with decimals
				{
					getline(myfile, inputString); //X, Y
					ss = std::stringstream(inputString);
					ss >> temp;
					x = std::stof(temp);
					ss >> temp;
					y = std::stof(temp);

					getline(myfile, inputString); //U, V
					ss = std::stringstream(inputString);
					ss >> temp;
					u = std::stoi(temp);
					ss >> temp;
					v = std::stoi(temp);

					xy[i] = glm::vec2(x, y);
					uv[i] = glm::vec2(u, v);
					xy[i] = fileCoordToScreenSpace(xy[i]);
				}

					getline(myfile, inputString); //tex1
					textureId1 = std::stoi(inputString);
					getline(myfile, inputString); //tex2
					textureId2 = std::stoi(inputString);
					getline(myfile, inputString); //uniqueKey
					uniqueKey = std::stoi(inputString);
					getline(myfile, inputString); //scale
					scale = std::stoi(inputString);

				if (convertedResult == 0)
				{
					buttons[counterButtons] = Button(xy, uv, textureId1, textureId2, counter, uniqueKey);
					buttons[counterButtons].scalePositions(scale);
					objIdList[counter] = counterButtons;
					textureList[counter] = textureId1;
					counterButtons++;
					result = true;
				}
				else
				{
					//SLider
					sliders[counterSliders] = Slider(xy, uv, textureId1, textureId2, counter, uniqueKey);
					sliders[counterButtons].scalePositions(scale, counter);
					objIdList[counter] = counterSliders;
					textureList[counter] = textureId1;
					counter++;
					//Button 2 that will be made in the slider class
					sliders[counterButtons].scalePositions(scale, counter);
					objIdList[counter] = counterSliders;
					textureList[counter] = textureId2;
					counterSliders++;
					result = true;
				}
			}
			else if (convertedResult == 2) //StaticText class
			{
				std::string temp;
				glm::vec2 xy[4];
				glm::vec2 uv[4];
				std::stringstream ss("");

				for (int i = 0; i < 4; i++)
				{
					getline(myfile, inputString); //X, Y
					ss = std::stringstream(inputString);
					ss >> temp;
					x = std::stof(temp);
					ss >> temp;
					y = std::stof(temp);

					getline(myfile, inputString); //U, V
					ss = std::stringstream(inputString);
					ss >> temp;
					u = std::stoi(temp);
					ss >> temp;
					v = std::stoi(temp);

					xy[i] = glm::vec2(x, y);
					uv[i] = glm::vec2(u, v);
				}

				getline(myfile, inputString); //tex1
				textureId1 = std::stoi(inputString);

				getline(myfile, inputString); //scale
				scale = std::stoi(inputString);

				staticText[counterStaticText] = StaticBox(xy, uv, textureId1, counter);
				staticText[counterStaticText].scalePositions(scale);
				objIdList[counter] = counterStaticText;
				textureList[counter] = textureId1;
				counterStaticText++;
				result = true;
			}
			else if (convertedResult == 3) //DynamicTextBox class
			{

				objIdList[counter] = counterDynamicBox;
				counterDynamicBox++;
				result = true;
			}
			else if (convertedResult == 4) //InputBox class
			{

				objIdList[counter] = counterInputBoxes;
				counterInputBoxes++;
				result = true;
			}
			else //Error message.
			{
				//std::cout << "Error: Invalid class key" << endl;
			}

			counter++;
		}
	}
	else
		//std::cout << "Error: File could not be opened." << std::endl;

	myfile.close();

	return result;
}

int UI::mouseCollission(glm::vec2 pos) 
{
	int hit = -1;
	int result = -1;

	for (int i = 0; i < nrOfbuttons && hit != -1; i++)
	{
		hit = buttons[i].checkCollision(pos);
	}
	for (int i = 0; i < nrOfsliders && hit != -1; i++)
	{
		hit = sliders[i].checkCollision(pos);
	}
	//for (int i = 0; i < nrOfdynamicTextBoxes && hit != -1; i++)
	//{
	//	hit = dynamicTextBoxes[i].checkCollision(pos);
	//}

	if (hit != -1)
		result = collisionEvent(hit);

	return result;
}

//Continue adding on buttons events
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

glm::vec2 UI::fileCoordToScreenSpace(glm::vec2 pos)
{
	glm::vec2 newVec2;

	newVec2.x = pos.x * 2 - 1;
	newVec2.y = pos.y * 2 - 1;

	return newVec2;
}

void UI::setWorldMatrix(float x, float y, int objId)
{
	sliders[objIdList[objId]].setWorldMatirx(x, y);
}
glm::mat4 UI::returnWorldMatrix(int objId)
{
	int index = objIdList[objId];
	glm::mat4 tmp = { 1, 0, 0, 0,
					  0, 1, 0, 0,
					  0, 0, 1, 0,
					  0, 0, 0, 0
						};

	if (index >= 0 && index < nrOfbuttons)
	{
		tmp = buttons[index].returnWorldMatrix();
	}
	else if (index >= nrOfbuttons && index < nrOfsliders)
	{
		tmp = sliders[index].returnWorldMatrix(objId);
	}
	else if (index >= nrOfsliders && index < nrOfstaticText)
	{
		tmp = staticText[index].returnWorldMatrix();
	}
	else if (index >= nrOfstaticText && index < nrOfdynamicTextBoxes)
	{
		//tmp = dynamicTextBoxes[index].returnWorldMatrix();
	}
	else if (index >= nrOfdynamicTextBoxes && index < nrOfinputBoxes)
	{
		//tmp = inputBoxes[index].returnWorldMatrix();
	}

	return tmp;
}

Vertex* UI::returnPosAUv(int id)
{
	int index = objIdList[id];
	Vertex* tmp = nullptr;
	
	if (index >= 0 && index < nrOfbuttons)
	{
		tmp = buttons[index].returnPosAUv();
	}
	else if (index >= nrOfbuttons && index < nrOfsliders)
	{
		tmp = sliders[index].returnPosAUv(id);
	}
	else if (index >= nrOfsliders && index < nrOfstaticText)
	{
		tmp = staticText[index].returnPosAUv();
	}
	else if (index >= nrOfstaticText && index < nrOfdynamicTextBoxes)
	{
		//tmp = dynamicTextBoxes[index].returnPosition();
	}
	else if (index >= nrOfdynamicTextBoxes && index < nrOfinputBoxes)
	{
		//tmp = inputBoxes[index].returnPosition();
	}

	return tmp;
}
int* UI::returnTextureList()
{
	return textureList;
}
int UI::returnObjCount()
{
	return nrOfObjects;
}

int UI::changeTex(int objId)
{
	int index = objIdList[objId];

	return buttons[index].changeTexUsed();
}