#ifndef STATICTEXTUREBOXES_H
#define STATICTEXTUREBOXES_H

#include <GL\glew.h>
#include "UIElements.h"

enum BannerTextureIDs
{
	Victory,
	Defeat,
	ParkinAreaActive,
	MarketActive,
	HoldYourGround,
	FinalAssult,
	EndOfRound
};

enum ScoreAdderTextureIDs
{
	ScoreAdder1,
	ScoreAdder2,
	ScoreAdder3
};

class StaticTextureBoxes : public UIElements
{
private:
	IRenderPipeline* uiRender;
	glm::vec2 center;
	glm::vec2* textureRes;
	int* textureIndexList;
	int textureInUse;
	int uniqueKey;
	std::vector<glm::mat4> worldMatrix;
	glm::mat4 originalWM;
	int nrOfTextures;
	glm::vec3 pivot;

	int winX;
	int winY;
	float startWMX;
	float startWMY;

public:
	StaticTextureBoxes();
	StaticTextureBoxes(glm::vec2 center, int* textureId1, int nrOfTextures, IRenderPipeline* uiRender, std::vector<glm::vec2> textRes);
	virtual ~StaticTextureBoxes();

	virtual void renderQuad(int id);
	virtual void renderText(int id);

	virtual void setWorldMatrix(int id, float x, float y);
	virtual void resetWorldMatrix(int id);

	int addNewWM();
	void deleteOldestWM();

	virtual void changeTexUsed(int id);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos);

	virtual void hoverCheck(glm::vec2 pos);

	virtual void setTexture(std::vector<GLuint> uiTextureIds);

	virtual void scaleBar(int id, float procentOfMax, bool fromRight);

	virtual void setWindowResolution(int winX, int winY);

	virtual void setText(std::string text);

	virtual std::string getText();
	virtual void removeLastInput();
	virtual void cleanText();
};

#endif