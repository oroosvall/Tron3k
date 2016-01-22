#pragma once
#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "UI.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "../Console.h"
#include "uiVertex.h"

class UIManager
{
private:
	UI* menus;
	int nrOfMenus;
	int maxMenus;
	int currentMenu;

	std::string* fileNamesList;
	int nrOfFileNames;

	Console* console;

public:
	UIManager();
	~UIManager();

	void init(std::string fileName, Console console);

	//Add and remove menus
	bool addMenu(int fileId);
	bool removeMenu(int menuId);
	void removeAllMenus();

	int collisionCheck(glm::vec2 pos);

	int returnObjCount();
	uiVertex* returnPosAUv(int id);
	int* returnTextureList();
	glm::mat4* returnWorldMatrix(int id);

	int changeTex(int objId);
};

#endif