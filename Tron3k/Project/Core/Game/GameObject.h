#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm/glm.hpp>

class GameObject
{
protected:
	glm::mat4 worldMat = { 1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1 };
	int contentIndex;
public:
	glm::mat4* getWorldMat() { return &worldMat; };
	GameObject() { contentIndex = -1; };
	~GameObject() {};
	void init(int idi) {};

	void rotate(float x, float y, float z)
	{
		glm::vec3 pos = glm::vec3(worldMat[0].w, worldMat[1].w, worldMat[2].w);

		//move to origo
		worldMat[0].w -= pos.x;
		worldMat[1].w -= pos.y;
		worldMat[2].w -= pos.z;

		//rot X
		worldMat *= glm::mat4(1, 0.0f, 0.0f, 0.0f,
			0.0f, cos(x), -sin(x), 0.0f,
			0.0f, sin(x), cos(x), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		//rot Y
		worldMat *= glm::mat4(cos(y), 0.0f, -sin(y), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sin(y), 0.0f, cos(y), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		//rot Z
		worldMat *= glm::mat4(cos(z), -sin(z), 0.0f, 0.0f,
			sin(z), cos(z), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		//move back to worldspace pos
		worldMat[0].w += pos.x;
		worldMat[1].w += pos.y;
		worldMat[2].w += pos.z;
	}

	void resetRotation()
	{
		glm::vec3 pos = glm::vec3(worldMat[0].w, worldMat[2].w, worldMat[2].w);

		worldMat = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0);

		//move back to worldspace pos
		worldMat[0].w += pos.x;
		worldMat[1].w += pos.y;
		worldMat[2].w += pos.z;
	}
};

#endif