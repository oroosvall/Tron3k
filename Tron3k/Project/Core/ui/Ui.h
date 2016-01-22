#pragma once
#ifndef UI_H
#define UI_H

#include "Button.h"
#include "Slider.h"
#include "staticBox.h"
#include "InputBox.h"
#include "DynamicTextBox.h"
#include "uiVertex.h"

#include "../Console.h"

#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class UI
{
private:
	Button* buttons;
	Slider* sliders;
	StaticBox* staticText;
	//DynamicTextBox* dynamicTextBoxes;
	//InputBox* inputBoxes;

	int* objIdList; //Used to change from the renders list to UI's list of objects.
	int* textureList;

	int nrOfbuttons;
	int nrOfsliders;
	int nrOfstaticText;
	int nrOfdynamicTextBoxes;
	int nrOfinputBoxes;
	int nrOfObjects;
	int menuId;

	Console* console;

	glm::mat4* tmpMat;

public:
	UI();
	UI(Console* console);
	~UI();

	void clean();
	void changeMenuId();

	bool loadUI(std::string fileName);
	int mouseCollission(glm::vec2 pos);
	int collisionEvent(int UniqueKey);
	glm::vec2 fileCoordToScreenSpace(glm::vec2 pos);

	void setWorldMatrix(float x, float y, int objId);
	glm::mat4* returnWorldMatrix(int objId);
	uiVertex* returnPosAUv(int id);
	int* returnTextureList();
	int returnObjCount();
	int changeTex(int objId);

	//Hover function, den ska ändra på textureList eftersom en buttons texture ändras
};

#endif