#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <GL\glew.h>
#include "UIElements.h"

class InputBox : public UIElements
{
private:
	IRenderPipeline* uiRender;
	glm::vec2 pos[2];
	glm::vec2 center;

	glm::vec2 textureRes;
	int textureIdList[2];
	int textureIdInUse;

	int uniqueKey;

	glm::mat4 worldMatrix;

	int hoverCheckKey = 0;

	bool active;

	glm::vec3 pivot;

	int text;
	std::string menuInputText;
	std::string ingameText;
	int outPutLength;

	int winX;
	int winY;

	int yOffSet;
	int xOffSet;

	float startWMX;
	float startWMY;
public:
	InputBox();
	InputBox(glm::vec2 center, int textureId1, int textureId2, int uniqueKey, IRenderPipeline* uiRender, glm::vec2 textRes, int winX, int winY, glm::vec3 offSetsTextSize);
	virtual ~InputBox();

	virtual void renderQuad(int id);
	virtual void renderText(int id);

	virtual void setWorldMatrix(int id, float x, float y);
	virtual void resetWorldMatrix(int id);

	virtual void changeTexUsed(int id, int wmID);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos, float &newSoundProcent, float &xPos);

	virtual void hoverCheck(glm::vec2 mpos);

	virtual void setTexture(std::vector<GLuint> uiTextureIds);

	virtual void scaleBar(int id, float procentOfMax, bool fromRight);

	virtual void setWindowResolution(int winX, int winY);

	virtual void setText(std::string text);

	virtual std::string getText();
	virtual void removeLastInput();
	virtual void cleanText();
};

#endif