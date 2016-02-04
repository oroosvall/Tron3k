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
	int textureId;

	int uniqueKey;

	glm::mat4 worldMatrix;

	int hoverCheckKey = 0;

	bool active;

	glm::vec3 pivot;

	int text;

	int winX;
	int winY;

	int offset;
public:
	InputBox();
	InputBox(glm::vec2 center, int textureId1, int uniqueKey, IRenderPipeline* uiRender, glm::vec2 textRes, int winX, int winY);
	virtual ~InputBox();

	virtual void render(int id);

	virtual void setWorldMatrix(float x, float y, int id);

	virtual void changeTexUsed(int id);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos);

	virtual void hoverCheck(glm::vec2 mpos);

	virtual void setTexture(std::vector<GLuint> uiTextureIds);

	virtual void scaleBar(float procentOfMax, bool fromRight);

	virtual void setWindowResolution(int winX, int winY);

	virtual void setText(std::string text);

	bool activeOrNot();
};

#endif