#include"CollideMesh.h"

CollideMesh::CollideMesh()
{
	init();
}

CollideMesh::~CollideMesh()
{

}

void CollideMesh::init()
{
	setAABB(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	getCylinderFromAABB();
	getSphereFromAABB();
	getAngCylinderFromAABB();
}

void CollideMesh::setAABB(glm::vec3 pos, glm::vec3 max, glm::vec3 min)
{
	boundingBox.pos = pos;
	boundingBox.max = max;
	boundingBox.min = min;

	boundingBox.ObbBoxes.clear();

	OBB temp;
	temp.corners[DLF] = glm::vec3(min.x/* + pos.x*/, min.y/* + pos.y*/, max.z/* + pos.z*/);
	temp.corners[DRF] = glm::vec3(max.x/* + pos.x*/, min.y/* + pos.y*/, max.z/* + pos.z*/);
	temp.corners[ULF] = glm::vec3(min.x/* + pos.x*/, max.y/* + pos.y*/, max.z/* + pos.z*/);
	temp.corners[URF] = glm::vec3(max.x/* + pos.x*/, max.y/* + pos.y*/, max.z/* + pos.z*/);
	temp.corners[DLB] = glm::vec3(min.x/* + pos.x*/, min.y/* + pos.y*/, min.z/* + pos.z*/);
	temp.corners[DRB] = glm::vec3(max.x/* + pos.x*/, min.y/* + pos.y*/, min.z/* + pos.z*/);
	temp.corners[ULB] = glm::vec3(min.x/* + pos.x*/, max.y/* + pos.y*/, min.z/* + pos.z*/);
	temp.corners[URB] = glm::vec3(max.x/* + pos.x*/, max.y/* + pos.y*/, min.z/* + pos.z*/);

	boundingBox.ObbBoxes.push_back(temp);

	//getCylinderFromAABB();
}

//sets up collidemesh from a Loaded collision mesh

void CollideMesh::setAABB(AABBloaded* aabb)
{
	boundingBox.init(aabb);
}

void CollideMesh::setAABB(AABB aabb)
{
	//We just make sure the information is copied, JUST IN CASE, to avoid memory corruption
	boundingBox.pos = aabb.pos;
	boundingBox.max = aabb.max;
	boundingBox.min = aabb.min;

	boundingBox.ObbBoxes.clear();

	for (int i = 0; i < aabb.ObbBoxes.size(); i++)
	{
		OBB temp;
		for (int j = 0; j < 8; j++)
		{
			temp.corners[j] = aabb.ObbBoxes[i].corners[j];
		}

		boundingBox.ObbBoxes.push_back(temp);
	}
}

AABB* CollideMesh::getAABB()
{
	return &boundingBox;
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

void CollideMesh::setSphere(glm::vec3 pos, float radius)
{
	sphere.pos = pos;
	sphere.radius = radius;
}

void CollideMesh::setSphere(Sphere sphere)
{
	this->sphere = sphere;
}

Sphere CollideMesh::getSphere()
{
	return sphere;
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

void CollideMesh::getAngCylinderFromAABB()
{
	angledCylinder.pos = boundingBox.pos;
	//rotation is impossible from an AABB, so default is (0,1,0)
	angledCylinder.normal = vec3(0, 1, 0);

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

void CollideMesh::getSphereFromAABB()
{
	sphere.pos = boundingBox.pos;

	float maxY = 0;

	//Calculates biggest possible radius for sphere
	if (abs(boundingBox.max.y - boundingBox.pos.y) > abs(boundingBox.min.y - boundingBox.pos.y))
		maxY = abs(boundingBox.max.y - boundingBox.pos.y);
	else
		maxY = abs(boundingBox.min.y - boundingBox.pos.y);

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
	if (maxY > grt)
		grt = maxY;

	sphere.radius = grt;
}

void CollideMesh::setPos(glm::vec3 pos)
{
	boundingBox.pos = glm::vec3(pos);

	cylinder.pos = pos;

	sphere.pos = pos;

	//capBox.pos = vec4(pos, 1);

	angledCylinder.pos = pos;
}
