#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "UIElements.h"

class Button : public UIElements
{
private:
	IRenderPipeline* uiRender;
	glm::vec2 pos[2];
	glm::vec2 textureRes;
	glm::vec2 center;
	int textureIndexList[2];
	int textureIndexInUse;
	int uniqueKey;
	glm::mat4 worldMatrix;

public:
	Button();
	Button( glm::vec2 center, int textureId1, int textureId2, int uniqueKey, IRenderPipeline* uiRender, glm::vec2 textRes);
	~Button();

	virtual void render(int id);

	virtual void setWorldMatrix(float x, float y, int id);

	virtual void changeTexUsed();

	virtual void scalePositions(int scale, int id);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos);
};

#endif