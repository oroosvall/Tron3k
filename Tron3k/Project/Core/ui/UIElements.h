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
	virtual void render(int id) = 0;

	virtual void setWorldMatrix(float x, float y, int id) = 0;

	virtual void changeTexUsed() = 0;

	virtual void scalePositions(int scale, int id) = 0;

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id) = 0;

	virtual int checkCollision(glm::vec2 pos) = 0;

	//virtual uiVertex* returnPosAUv(int id) = 0;
};

#endif