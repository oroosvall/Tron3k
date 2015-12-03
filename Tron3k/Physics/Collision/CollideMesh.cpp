#include"CollideMesh.h"

CollideMesh::CollideMesh()
{

}

CollideMesh::~CollideMesh()
{

}

void CollideMesh::init()
{
	setAABB(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	setCylinder(glm::vec3(0, 0, 0), 0.5f, 1.0f);
}

void CollideMesh::setAABB(glm::vec3 pos, glm::vec3 max, glm::vec3 min)
{
	//boundingBox.pos = pos;
	//boundingBox.max = max;
	//boundingBox.min = min;
}

AABB CollideMesh::getAABB()
{
	return boundingBox;
}

void CollideMesh::setCylinder(glm::vec3 pos, float radius, float height)
{
	cylinder.pos = pos;
	cylinder.radius = radius;
	cylinder.height = height;
}

Cylinder CollideMesh::getCylinder()
{
	return cylinder;
}

void CollideMesh::setPos(glm::vec3 pos)
{
	//boundingBox.pos = pos;

	cylinder.pos = pos;
}
