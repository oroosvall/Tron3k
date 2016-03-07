#ifndef HIDEABLE_H
#define HIDEABLE_H

#include <GL\glew.h>
#include "../../../RenderPipeline/IRenderPipeline.h"

#include "uiVertex.h"

#include <string>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

enum BannerTextureIDs
{
	Victory,
	Defeat,
	ParkinAreaActive,
	MarketActive,
	HoldYourGround,
	FinalAssult,
	EndOfRound,
	WarmUp
};

enum ScoreAdderTextureIDs
{
	ScoreAdder1,
	ScoreAdder2,
	ScoreAdder3
};

class HideAble
{
private:
	IRenderPipeline* uiRender;
	glm::vec2 center;
	glm::vec2* textureRes;
	int* textureIndexList;
	int textureInUse;
	std::vector<glm::mat4> worldMatrix;
	glm::mat4 originalWM;
	int nrOfTextures;
	glm::vec3 pivot;

	int winX;
	int winY;
	float startWMX;
	float startWMY;
public:
	HideAble();
	HideAble(glm::vec2 center, int* textureId1, int nrOfTextures, IRenderPipeline* uiRender, std::vector<glm::vec2> textRes, glm::vec3 offSetsTextSize);
	~HideAble();

	void renderQuad(int id);
	void renderText(int id);

	void setWorldMatrix(int id, float x, float y);
	void resetWorldMatrix(int id);

	int addNewWM();
	void deleteOldestWM();

	void changeTexUsed(int id, int wmID);

	void setTexture(std::vector<GLuint> uiTextureIds);

	/*
	void setText(std::string text);

	std::string getText();
	void removeLastInput();
	void cleanText();
	*/
};

#endif