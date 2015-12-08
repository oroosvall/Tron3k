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
	getCylinderFromAABB();
}

void CollideMesh::setAABB(glm::vec3 pos, glm::vec3 max, glm::vec3 min)
{
	boundingBox.pos = glm::vec4(pos, 1.0f);
	boundingBox.max = glm::vec4(max, 1.0f);
	boundingBox.min = glm::vec4(min, 1.0f);

	boundingBox.ObbBoxes.clear();
	OBB temp;
	temp.corners[0] = glm::vec4(min.x, min.y, max.z, 1.0f);
	temp.corners[1] = glm::vec4(max.x, min.y, max.z, 1.0f);
	temp.corners[2] = glm::vec4(min.x, max.y, max.z, 1.0f);
	temp.corners[3] = glm::vec4(max.x, max.y, max.z, 1.0f);
	temp.corners[4] = glm::vec4(min.x, min.y, min.z, 1.0f);
	temp.corners[5] = glm::vec4(max.x, min.y, min.z, 1.0f);
	temp.corners[6] = glm::vec4(min.x, max.y, min.z, 1.0f);
	temp.corners[7] = glm::vec4(max.x, max.y, min.z, 1.0f);

	boundingBox.ObbBoxes.push_back(temp);

	getCylinderFromAABB();
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

	getCylinderFromAABB();
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

void CollideMesh::getCylinderFromAABB()
{
	cylinder.pos = glm::vec3(boundingBox.pos);

	float maxY = 0;

	//Calculates max height for the cylinder
	if (abs(boundingBox.max.y - boundingBox.pos.y) > abs(boundingBox.min.y - boundingBox.pos.y))
		maxY = abs(boundingBox.max.y - boundingBox.pos.y);
	else
		maxY = abs(boundingBox.min.y - boundingBox.pos.y);

	cylinder.height = maxY;

	//Calculates the rad for the cylinder, so that it encapsulates the entire boundingBox
	float maxX = 0;
	float minX = 0;
	float maxZ = 0;
	float minZ = 0;
	maxX = abs(boundingBox.max.x - boundingBox.pos.x);
	minX = abs(boundingBox.min.x - boundingBox.pos.x);
	maxZ = abs(boundingBox.max.z - boundingBox.pos.z);
	minZ = abs(boundingBox.min.z - boundingBox.pos.z);

	float grt = maxX;

	if (minX > grt)
		grt = minX;
	if (maxZ > grt)
		grt = maxZ;
	if (minZ > grt)
		grt = minZ;

	cylinder.radius = grt;
}

void CollideMesh::setPos(glm::vec3 pos)
{
	boundingBox.pos = glm::vec4(pos, 1.0f);

	cylinder.pos = pos;
}
