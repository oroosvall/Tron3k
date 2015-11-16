#ifndef GEOMETRY_H
#define GEOMETRY_H

#include"../../Dependencies/glm/glm/glm.hpp"

class Geometry
{
private:
	//Geometry boxes go here
	//TODO: Load a bounding box, probably through the way we load files

	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 size = glm::vec3(0, 0, 0);
public:
	Geometry(glm::vec3 pos, glm::vec3 size);
	~Geometry();

	const glm::vec3 getPos() { return pos; }
	const glm::vec3 getSize() { return size; }

	void setPos(glm::vec3 pos) { this->pos = pos; }
	void setSize(glm::vec3 size) { this->size = size; }

};
#endif