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

enum scaleAndText
{
	HP, //Dynamic text and scaleBar
	Ammo,  //Dynamic text
	TicketBar1,  //Dynamic text and ScaleBar
	TicketBar2,  //Dynamic text and ScaleBar
	Wins1, //Dynamic text
	Wins2, //Dynamic text
	Time, //Dynamic text
	IP, //Dynamic text and inpit
	Name, //Dynamic text and input
	AbilityMeter, //ScaleBar
	LoseTicketsMeter //ScaleBar
};

class UI
{
private:
	IRenderPipeline *uiRender;

	//Lists
	std::vector<glm::vec2>* textureRes;
	std::vector<UIElements*> UiObjects;
	std::vector<int> textObjRenderId;
	int* textureIdList;

	//Counters
	int nrOfObjects;
	int nrOfObjectsToRender;

	//ids
	int menuId;
	int textIdList[11];

	Console* console;

	//	Functions
	//Load from file
	bool loadUI(std::string fileName, int winX, int winY);

	void removeMenu();
	//Clean up
	//Convert
	glm::vec2 fileCoordToScreenSpace(glm::vec2 pos);

	
public:
	UI();
	~UI();

	//Start
	void init(std::string fileName, Console* console, IRenderPipeline* uiRender, std::vector<glm::vec2>* textureRes, int winX, int winY);
	
	void renderMenu();
	void renderIngame();

	void clean();

	void changeTex(int objId, int whichTex);

	int mouseCollission(glm::vec2 pos);
	void mouseHover(glm::vec2 pos);

	void changeColorTeam(int whichTex);

	void hideWindow();

	void setTextureId(std::vector<GLuint> uiTextureIds);

	void setWindowResolution(int winX, int winY);

	void setText(std::string text, int id);
	std::string getText(int id);
	void removeLastInput(int id);
	void clearText(int id);

	void scaleBar(int id, float procentOfMax, bool fromRight);
};

#endif