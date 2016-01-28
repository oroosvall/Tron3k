#pragma once
#ifndef UI_H
#define UI_H

#include <GL\glew.h>

#include "UIElements.h"
#include "Button.h"
#include "VertexBufferUI.h"
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
	IRenderPipeline *uiRender;

	//Lists
	std::vector<glm::vec2>* textureRes;
	std::vector<UIElements*> UiObjects;
	int* textureIdList;

	//Counters
	int nrOfObjects;
	int nrOfObjectsToRender;

	int menuId;

	Console* console;

	//	Functions
	//Load from file
	bool loadUI(std::string fileName);

	void removeMenu();
	//Clean up
	//Convert
	glm::vec2 fileCoordToScreenSpace(glm::vec2 pos);
	//Events
	int collisionEvent(int UniqueKey);
public:
	UI();
	~UI();

	//Start
	void init(std::string fileName, Console* console, IRenderPipeline* uiRender, std::vector<glm::vec2>* textureRes);
	
	void render(std::vector<GLuint> uiTextureIds);

	void clean();

	int mouseCollission(glm::vec2 pos);
	
	void changeTex(int objId);

	void hideWindow();

	void setWorldMatrix(float x, float y, int objId);

	void setTextureId(std::vector<GLuint> uiTextureIds);
};

#endif