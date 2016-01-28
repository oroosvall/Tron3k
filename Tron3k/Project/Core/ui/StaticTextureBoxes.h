#ifndef STATICTEXTUREBOXES_H
#define STATICTEXTUREBOXES_H

#include <GL\glew.h>
#include "UIElements.h"

class StaticTextureBoxes : public UIElements
{
private:
	IRenderPipeline* uiRender;
	uiVertex pos;
	int textureIndexList;
	int uniqueKey;
	glm::mat4 worldMatrix;

public:
	StaticTextureBoxes();
	StaticTextureBoxes(glm::vec2 positions[], glm::vec2 uv[], int textureId1, IRenderPipeline* uiRender);
	~StaticTextureBoxes();

	virtual void render(int id);

	virtual void setWorldMatrix(float x, float y, int id);

	virtual void changeTexUsed();

	virtual void scalePositions(int scale, int id);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos);

	virtual void setTexture(std::vector<GLuint> uiTextureIds);
};

#endif