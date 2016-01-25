#pragma once
#ifndef UI_H
#define UI_H

#include "uiVertex.h"

#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class UI
{
private:

	int* objIdList; //Used to change from the renders list to UI's list of objects.
	int* textureList;

	int nrOfbuttons;
	int nrOfsliders;
	int nrOfstaticText;
	int nrOfdynamicTextBoxes;
	int nrOfinputBoxes;
	int nrOfObjects;
	int menuId;

public:
	UI();
	~UI();

	void clean();
	void changeMenuId();

	bool loadUI(std::string fileName);
	int mouseCollission(glm::vec2 pos);
	int collisionEvent(int UniqueKey);
	glm::vec2 fileCoordToScreenSpace(glm::vec2 pos);

	void setWorldMatrix(float x, float y, int objId);
	glm::mat4 returnWorldMatrix(int objId);
	uiVertex* returnPosAUv(int id);
	int* returnTextureList();
	int returnObjCount();
	int changeTex(int objId);

	//Hover function, den ska ändra på textureList eftersom en buttons texture ändras
};

#endif