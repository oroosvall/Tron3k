#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "UIElements.h"

class Button : public UIElements
{
private:
	uiVertex pos;
	int textureIndexList[2];
	int textureIndexInUse;
	int uniqueKey;
	glm::mat4 worldMatrix;

public:
	Button();
	Button( glm::vec2 positions[], glm::vec2 uv[], int textureId1, int textureId2, int uniqueKey);
	~Button();

	virtual void render(int id);

	virtual void setWorldMatrix(float x, float y, int id);

	virtual void changeTexUsed();

	virtual void scalePositions(int scale, int id);

	virtual void fromPosToQuadScreen();

	virtual int checkCollision(glm::vec2 pos);
};

#endif