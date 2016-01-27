#pragma once
#ifndef SLIDER_H
#define SLIDER_H

#include "UIElements.h"

class Slider : public UIElements
{
private:
	IRenderPipeline* uiRender;
	uiVertex pos[2];
	int textureIndexList[2];
	int textureIndexInUse;
	int uniqueKey;
	glm::mat4 worldMatrix[2];
	int nrOfButtons;
	int objId[2];

	void createAdditionalPoint();
public:
	Slider();
	Slider(glm::vec2 positions[], glm::vec2 uv[], int textureId1, int textureId2, int uniqueKey, int objId1, int objId2, IRenderPipeline* uiRender);
	~Slider();

	virtual void render(int id);

	virtual void setWorldMatrix(float x, float y, int id);

	virtual void changeTexUsed();

	virtual void scalePositions(int scale, int id);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos);
};

#endif

//Sätt button direkt dit spelaren klickade.