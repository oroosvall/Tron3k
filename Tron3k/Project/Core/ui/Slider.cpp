#include "Slider.h"

Slider::Slider() {}
Slider::Slider(glm::vec2 positions[], glm::vec2 uv[], int tex1, int tex2, int objId, int uniqueKey) //Not done
{
	backGround = Button(positions, uv, tex1, tex2, objId, uniqueKey);

	//create pointer, objId+1, uniqueKey+1
}
Slider::~Slider() {}

void Slider::setWorldMatirx(float x, float y) 
{
	pointer.setWorldMatrix(x, y);
}

glm::mat4 Slider::returnWorldMatrix(int objId) 
{ 
	glm::mat4 result;
	if (objId == backGround.returnObjId())
		result = backGround.returnWorldMatrix();
	else
		result = pointer.returnWorldMatrix();

	return result;
}

Vertex* Slider::returnPosAUv(int objId)
{
	Vertex* tmpVertex;

	if (objId == backGround.returnObjId())
		tmpVertex = backGround.returnPosAUv();
	else
		tmpVertex = pointer.returnPosAUv();

	return tmpVertex;
}

int Slider::calcSound() { return 0;  } //Not done

bool Slider::checkCollision(glm::vec2 pos) { return false; } //Not done

void Slider::scalePositions(int scale, int objId)
{
	if (objId == backGround.returnObjId())
		backGround.scalePositions(scale);
	else
		pointer.scalePositions(scale);
}