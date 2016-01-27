#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "UIElements.h"

class Button : public UIElements
{
private:
	uiVertex pos[2];
	int textureIndexList[2];
	int textureIndexInUse;
	int uniqueKey;
	glm::mat4 worldMatrix[2];
	int nrOfButtons;

	GLuint uniformTextureLocation;
	GLuint uniformWorldMatrix;
	GLuint shader;

public:
	Button();
	Button( glm::vec2 positions[], glm::vec2 uv[], int textureId1, int textureId2, int uniqueKey, GLuint shader, GLuint worldMat, GLuint textureLocation);
	//Button(...) //Constructor for sliders
	~Button();

	void render(std::vector<GLuint> textureIds, int i, GLuint gVertexAttribute, GLuint gVertexBuffer);

	void setWorldMatrix(float x, float y, int id);

	void changeTexUsed();

	void scalePositions(int scale, int id);

	int checkCollision(glm::vec2 pos);

	uiVertex returnPosAUv();
};

#endif