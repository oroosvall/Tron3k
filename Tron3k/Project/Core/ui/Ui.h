#pragma once
#ifndef UI_H
#define UI_H

#include <GL\glew.h>

#include "UIElements.h"
#include "Button.h"
#include "StaticTextureBoxes.h"
#include "InputBox.h"
#include "Slider.h"
#include "VertexBufferUI.h"
#include "uiVertex.h"
#include "HideAble.h"

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
	Time, //Dynamic text
	IP, //Dynamic text and inpit
	Name, //Dynamic text and input
	AbilityMeter, //ScaleBar
	LoseTicketsMeter, //ScaleBar
	Wins1, //Dynamic text
	Wins2, //Dynamic text
	Sense,  //Sensitivity
	consumable,
	weapon
};

enum hideAbleObj
{
	Banner,
	ScoreAdderTeam1,
	ScoreAdderTeam2,
	TicketReducerTeam1,
	TicketReducerTeam2,
};


class UI
{
private:
	//Other class objects
	IRenderPipeline *uiRender;
	Console* console;

	//Lists
	std::vector<glm::vec2>* textureRes;
	std::vector<UIElements*> UiObjects;
	std::vector<int> textObjIds;
	int* textureIdList;
	std::vector<int> twoColoredHUDObjects;

	//Counters
	int nrOfObjects;
	int nrOfObjectsToRender;

	//ids
	int menuId;
	int textIdList[14];

	//Hideable stuff
	std::vector<HideAble*> hideAbleObjects;
	std::vector<int> hideAbleIds;

	//	Functions
	//Load from file
	bool loadUI(std::string fileName, int winX, int winY);
	//Clean up
	void removeMenu();
	//Convert
	glm::vec2 fileCoordToScreenSpace(glm::vec2 pos);

	float* optionsSaved;
	bool optionsMenu;
	
public:
	UI();
	~UI();

	//Start
	void init(std::string fileName, Console* console, IRenderPipeline* uiRender, std::vector<glm::vec2>* textureRes, int winX, int winY);
	
	void renderMenu();
	void renderIngame();
	

	void clean();

	void changeTex(int objId, int whichTex);
	void scaleAndTextChangeTexture(int objId, int whichTex);

	int mouseCollission(glm::vec2 pos, float &newSoundProcent);
	void mouseHover(glm::vec2 pos);

	void changeColorTeam(int whichTex);

	void setTextureId(std::vector<GLuint> uiTextureIds);
	void resetAllObjsTexture();

	void setWindowResolution(int winX, int winY);

	void setText(std::string text, int id);
	std::string getText(int id);
	void removeLastInput(int id);
	void clearText(int id);

	void scaleBar(int id, float procentOfMax, bool fromRight);

	void setOptionsSaved(float* list);
	float* getOptionsSaved();

	void stopRendering(int id, bool r);

	//Hideable stuff
	void renderHideable();
	void hideObject(int id);
	void showObject(int id);
	void changeHideAbleTexture(int objId, int wmID, int whichTex);
	void setHideableWorldMatrix(int id, int wmId, glm::vec2 xy);
	void resetHidableWorldMatrix(int id, int wmId);
	int addNewWM(int id);
	void deleteOldestWM(int id);
};

#endif