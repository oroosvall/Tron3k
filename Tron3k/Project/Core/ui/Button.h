#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include <GL\glew.h>
#include "UIElements.h"


class Button : public UIElements
{
private:
	//Render pointer
	IRenderPipeline* uiRender;
	//Positions
	glm::vec2 pos[2];
	glm::vec2 center;
	//Textures
	glm::vec2 textureRes[2];
	int textureIdList[2];
	int textureIdInUse;
	//Event identifier
	int uniqueKey;
	//Matrix
	glm::mat4 worldMatrix;
	//Hover key
	int hoverCheckKey;
	
	bool dontChangeTexture;

	glm::vec3 pivot;

	int winX;
	int winY;
	float startWMX;
	float startWMY;

public:
	Button();
	Button(glm::vec2 center, int textureId1, int textureId2, int uniqueKey, int hoverKey, IRenderPipeline* uiRender, glm::vec2 textRes1, glm::vec2 textRes2);
	virtual ~Button();

	virtual void renderQuad(int id);
	virtual void renderText(int id);

	virtual void setWorldMatrix(int id, float x, float y);
	virtual void resetWorldMatrix(int id);

	virtual void changeTexUsed(int id, int wmID);
	void setDontChangeTexture(bool set);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos, float &newSoundProcent, float &xPos);

	virtual void hoverCheck(glm::vec2 mpos);

	virtual void setTexture(std::vector<GLuint> uiTextureIds);

	virtual void scaleBar(int id, float procentOfMax, bool fromRight);

	virtual void setWindowResolution(int winX, int winY);

	virtual void setText(std::string text);

	virtual std::string getText();
	virtual void removeLastInput();
	virtual void cleanText();
};

#endif