#pragma once
#ifndef UI_H
#define UI_H

#include <GL\glew.h>

#include "UIElements.h"
#include "Button.h"
#include "StaticTextureBoxes.h"
#include "InputBox.h"
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

	//ids
	int menuId;
	int textIdList[9];

	Console* console;

	//	Functions
	//Load from file
	bool loadUI(std::string fileName, int winX, int winY);

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
	void init(std::string fileName, Console* console, IRenderPipeline* uiRender, std::vector<glm::vec2>* textureRes, int winX, int winY);
	
	void render(std::vector<GLuint> uiTextureIds);

	void clean();

	int mouseCollission(glm::vec2 pos);
	void mouseHover(glm::vec2 pos);
	
	void changeTex(int objId, int whichTex);

	void hideWindow();

	void setWorldMatrix(float x, float y, int objId);

	void setTextureId(std::vector<GLuint> uiTextureIds);

	void setWindowResolution(int winX, int winY);

	void setText(std::string text, int id);
	std::string getText(int id);
	void removeLastInput(int id);
	void cleanText(int id);
};

#endif