#ifndef STATICTEXTUREBOXES_H
#define STATICTEXTUREBOXES_H

#include "UIElements.h"

class StaticTextureBoxes : public UIElements
{
private:
	uiVertex pos;
	int textureIndexList;
	int textureIndexInUse;
	int uniqueKey;
	glm::mat4 worldMatrix;

public:
	StaticTextureBoxes();
	StaticTextureBoxes(glm::vec2 positions[], glm::vec2 uv[], int textureId1);
	~StaticTextureBoxes();

	virtual void render(int id);

	virtual void setWorldMatrix(float x, float y, int id);

	virtual void changeTexUsed();

	virtual void scalePositions(int scale, int id);

	virtual void fromPosToQuadScreen();

	virtual int checkCollision(glm::vec2 pos);
};

#endif