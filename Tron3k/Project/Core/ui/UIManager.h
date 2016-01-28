#pragma once
#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "UI.h"
//#include "../../../RenderPipeline/IRenderPipeline.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


class UIManager
{
private:

	IRenderPipeline* renderPipe;

	UI* menus;
	int nrOfMenus;
	int maxMenus;
	int currentMenu;

	Console* console;

	//Lists of file names
	std::string* fileNamesListFirstGroup; //First set of menus(Those you can use before going ingame)
	std::string* fileNamesListSecondGroup; //Second set of menus(Those you can use while ingame)
	int nrOfFileNamesFirstGroup;
	int nrOfFileNamesSecondGroup;
	int currentGroup;

	//Render stuff
	std::vector<glm::vec2>* textureRes;
	std::vector<GLuint> uiTextureIds;
	std::vector<std::string> texturePaths;

	//	Functions
	void loadInTexture();

public:
	UIManager();
	~UIManager();

	void setRenderPtr(IRenderPipeline* ptr) 
	{ 
		renderPipe = ptr; 
	}

	void init(Console* console);
	
	void render();

	void setMenu(int menuId);
	void removeAllMenus();

	int collisionCheck(glm::vec2 pos);
	void hoverCheck(glm::vec2 pos);

	void changeTex(int objId, int whichTex);

	bool LoadNextSet(int whichMenuGroup);
};

#endif