#pragma once
#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "../../../RenderPipeline/IRenderPipeline.h"

#include "uiVertex.h"

#include <string>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class UIElements
{
public:
	virtual ~UIElements() {};

	virtual void renderQuad(int id) = 0;
	virtual void renderText(int id) = 0;

	virtual void setWorldMatrix(int id, float x, float y) = 0;
	virtual void resetWorldMatrix(int id) = 0;
	virtual float getWorldX() { return 0.0f; };

	virtual void changeTexUsed(int id, int wmID) = 0;
	//virtual void changeTexUsed(int textID, int textureID) = 0;
	virtual void setDontChangeTexture(bool set) {};

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id) = 0;

	virtual int checkCollision(glm::vec2 pos, float &newSoundProcent, float &xPos) = 0;

	virtual void hoverCheck(glm::vec2 pos) = 0;

	virtual void setTexture(std::vector<GLuint> uiTextureIds) = 0;

	virtual void scaleBar(int id, float procentOfMax, bool fromRight) = 0;

	virtual void setWindowResolution(int winX, int winY) = 0;

	virtual void setText(std::string text) = 0;
	virtual std::string getText() = 0;
	virtual void removeLastInput() = 0;
	virtual void cleanText() = 0;

	virtual float calculateSoundProcent(float mposX) { return 0; };

	virtual void dontRender(bool dont) {};

	//virtual uiVertex* returnPosAUv(int id) = 0;
};

#endif