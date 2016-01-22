#pragma once
#ifndef SLIDER_H
#define SLIDER_H

#include "Button.h"
#include "staticBox.h"

class Slider
{
private:
	Button backGround; //The button you click on.
	Button pointer; //The one that moves

public:
	Slider();
	Slider(glm::vec2 positions[], glm::vec2 uv[], int tex1, int tex2, int objId, int uniqueKey);
	~Slider();

	void setWorldMatirx(float x, float y);
	glm::mat4* returnWorldMatrix(int objId);
	uiVertex* returnPosAUv(int objId); //Kolla vilken av dom två knapparna det är

	int calcSound();

	bool checkCollision(glm::vec2 pos);

	void scalePositions(int scale, int objId);
};

#endif

//Sätt button direkt dit spelaren klickade.