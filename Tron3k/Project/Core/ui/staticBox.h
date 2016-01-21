#pragma once
#ifndef STATICBOX_H
#define STATICBOX_H

#include "Vertex.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class StaticBox
{
private:
	Vertex pos[4];
	int textureId;
	int objectId;

	glm::mat4 worldMatrix;
public:
	StaticBox();
	StaticBox( glm::vec2 positions[2], glm::vec2 UVs[2], int texId, int objId);
	~StaticBox();

	void setWorldMatrix(float x, float y);
	glm::mat4 returnWorldMatrix();
	Vertex* returnPosAUv();

	void changeTexUsed();

	void scalePositions(int scale);
};

#endif