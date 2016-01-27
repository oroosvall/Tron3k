#pragma once
#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "../../../Dependencies/glew-1.11.0/include/GL/glew.h"

#include "uiVertex.h"

#include <string>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class UIElements
{
public:
	virtual void render(std::vector<GLuint> textureIds, int i, GLuint gVertexAttribute, GLuint gVertexBuffer) = 0;

	virtual void setWorldMatrix(float x, float y, int id) = 0;

	virtual void changeTexUsed() = 0;

	virtual void scalePositions(int scale, int id) = 0;

	virtual int checkCollision(glm::vec2 pos) = 0;

	virtual uiVertex* returnPosAUv() = 0;
};

#endif