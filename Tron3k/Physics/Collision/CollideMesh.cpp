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
	boundingBox.pos = glm::vec4(pos, 1.0f);
	boundingBox.max = glm::vec4(max, 1.0f);
	boundingBox.min = glm::vec4(min, 1.0f);
}

void CollideMesh::setAABB(AABB aabb)
{
	//We just make sure the information is copied, JUST IN CASE, to avoid memory corruption
	boundingBox.pos = aabb.pos;
	boundingBox.max = aabb.max;
	boundingBox.min = aabb.min;

	for (int i = 0; i < aabb.ObbBoxes.size(); i++)
	{
		OBB temp;
		for (int j = 0; j < 8; j++)
		{
			temp.corners[j] = aabb.ObbBoxes[i].corners[j];
		}
		//temp.transform = aabb.ObbBoxes[i].transform;

		boundingBox.ObbBoxes.push_back(temp);
	}
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
	boundingBox.pos = glm::vec4(pos, 1.0f);

	cylinder.pos = pos;
}
