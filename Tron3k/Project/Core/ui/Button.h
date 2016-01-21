#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "Vertex.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Button
{
private:
	Vertex pos[4];
	int textureIndexList[2];
	int textureIndexInUse;
	int objId;
	int uniqueKey;
	glm::mat4 worldMatrix;

public:
	Button();
	Button( glm::vec2 positions[], glm::vec2 uv[], int textureId1, int textureId2, int objId, int uniqueKey);
	~Button();

	void setWorldMatrix(float x, float y);
	glm::mat4 returnWorldMatrix();
	Vertex* returnPosAUv();
	int returnObjId();

	int changeTexUsed();

	void scalePositions(int scale);

	int checkCollision(glm::vec2 pos);
};

#endif