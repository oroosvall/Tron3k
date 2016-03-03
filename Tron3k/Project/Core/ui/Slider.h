#pragma once
#ifndef SLIDER_H
#define SLIDER_H

#include <GL\glew.h>
#include "UIElements.h"

class Slider : public UIElements
{
private:
	IRenderPipeline* uiRender;
	glm::vec2 pos[4];
	glm::vec2 textureRes[2];
	glm::vec2 center[2];
	glm::mat4 worldMatrix[2];
	int textureIndexList[2];
	int uniqueKey;
	int nrOfButtons;
	float lastScale;

	glm::vec3 pivot;

	int winX;
	int winY;
	float startWMX;
	float startWMY;

	void createAdditionalPoint();
public:
	Slider();
	Slider(glm::vec2 center, int textureId1, int textureId2, int uniqueKey, IRenderPipeline* uiRender, glm::vec2 textRes, glm::vec2 textRes2);
	virtual ~Slider();

	virtual void renderQuad(int id);
	
	virtual void setWorldMatrix(int id, float x, float y);
	virtual void resetWorldMatrix(int id);
	virtual float getWorldX();

	virtual int checkCollision(glm::vec2 pos, float &newSoundProcent, float &xPos);

	virtual void setTexture(std::vector<GLuint> uiTextureIds);

	virtual void setWindowResolution(int winX, int winY);

	virtual float calculateSoundProcent(float mposX);

	// Not used
	virtual void renderText(int id); //Not used
	virtual void changeTexUsed(int id, int wmID); //Not used
	virtual void fromPosToQuadScreen(glm::vec2 positions, int id); //Not used
	virtual void hoverCheck(glm::vec2 pos); //Not used
	virtual void scaleBar(int id, float procentOfMax, bool fromRight); //Not used
	virtual void setText(std::string text); // not used
	virtual std::string getText();
	virtual void removeLastInput();
	virtual void cleanText();
};

#endif