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


public:
	Button();
	Button(glm::vec2 center, int textureId1, int textureId2, int uniqueKey, int hoverKey, IRenderPipeline* uiRender, glm::vec2 textRes1, glm::vec2 textRes2);
	~Button();

	virtual void render(int id);

	virtual void setWorldMatrix(float x, float y, int id);

	virtual void changeTexUsed(int id);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos);

	virtual void hoverCheck(glm::vec2 mpos);

	virtual void setTexture(std::vector<GLuint> uiTextureIds);


};

#endif