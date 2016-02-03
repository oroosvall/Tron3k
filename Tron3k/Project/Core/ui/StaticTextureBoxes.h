#ifndef STATICTEXTUREBOXES_H
#define STATICTEXTUREBOXES_H

#include <GL\glew.h>
#include "UIElements.h"

class StaticTextureBoxes : public UIElements
{
private:
	IRenderPipeline* uiRender;
	glm::vec2 center;
	glm::vec2* textureRes;
	int* textureIndexList;
	int textureInUse;
	int uniqueKey;
	glm::mat4 worldMatrix;
	int nrOfTextures;
	glm::vec3 pivot;

public:
	StaticTextureBoxes();
	StaticTextureBoxes(glm::vec2 center, int* textureId1, int nrOfTextures, IRenderPipeline* uiRender, std::vector<glm::vec2> textRes);
	virtual ~StaticTextureBoxes();

	virtual void render(int id);

	virtual void setWorldMatrix(float x, float y, int id);

	virtual void changeTexUsed(int id);

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id);

	virtual int checkCollision(glm::vec2 pos);

	virtual void hoverCheck(glm::vec2 pos);

	virtual void setTexture(std::vector<GLuint> uiTextureIds);

	virtual void scaleBar(float procentOfMax, bool fromRight);
};

#endif