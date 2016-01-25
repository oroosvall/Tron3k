#pragma once
#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "UI.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


class UIManager
{
private:
	UI* menus;
	int nrOfMenus;
	int maxMenus;
	int currentMenu;

	std::string* fileNamesList;
	int nrOfFileNames;
	//Render stuff
	//GLuint uniformtextureLocation;
	//GLuint uniformWorldMat;
	//GLuint shader;
	//std::vector<GLuint> uiTextureIds;
	std::vector<std::string> texturePaths;
public:
	UIManager();
	~UIManager();

	void init(std::string fileName);

	void update();
	void render();

	void setMenu(int menuId);
	void removeAllMenus();

	int collisionCheck(glm::vec2 pos, int whichMenu);

	int changeTex(int objId);

	bool Load(int whichMenuGroup);
};

#endif