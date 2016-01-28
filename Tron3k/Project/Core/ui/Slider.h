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
	int objId[2];

	void createAdditionalPoint();
public:
	Slider();
	Slider(glm::vec2 center, int textureId1, int textureId2, int uniqueKey, int objId1, int objId2, IRenderPipeline* uiRender, glm::vec2 textRes, glm::vec2 textRes2);
	~Slider();

	virtual void render(int id);

	virtual void setWorldMatrix(float x, float y, int id);

	virtual void changeTexUsed(int id);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos);

	virtual void hoverCheck(glm::vec2 pos);

	virtual void setTexture(std::vector<GLuint> uiTextureIds);
};

#endif

//Sätt button direkt dit spelaren klickade.