#include "staticBox.h"

StaticBox::StaticBox() {}
StaticBox::StaticBox(glm::vec2 positions[2], glm::vec2 UVs[2], int texId, int objId) {}
StaticBox::~StaticBox() {}

void StaticBox::setWorldMatrix(float x, float y)
{

}
glm::mat4 StaticBox::returnWorldMatrix()
{
	return worldMatrix;
}
Vertex* StaticBox::returnPosAUv()
{
	return pos;
}

void StaticBox::changeTexUsed() {}

void StaticBox::scalePositions(int scale) {}