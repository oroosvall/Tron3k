#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject
{
protected:
	//glm::mat4 worldMat = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	int contentIndex;
public:
	//glm::mat4* getWorldMat();
	GameObject() { contentIndex = -1; };
	~GameObject() {};
	void init(int idi);
	//int bindWorldMat(GLuint* shaderProgram, GLuint* shaderuniform) const;
};

#endif