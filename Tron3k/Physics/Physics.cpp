#include "Physics.h"

Physics::Physics()
{

}

Physics::~Physics()
{

}

bool Physics::init()
{
	return 1;
}

bool Physics::release()
{
	return 1;
}

bool Physics::checkCollision(Geometry* obj1, Geometry* obj2)
{
	//AABB collision mellan objekt
	if (obj1->getPos().x + obj1->getSize().x > obj2->getPos().x - obj2->getSize().x &&
		obj1->getPos().x - obj1->getSize().x < obj2->getPos().x + obj2->getSize().x)//x
	{
		if (obj1->getPos().y + obj1->getSize().y > obj2->getPos().y - obj2->getSize().y &&
			obj1->getPos().y - obj1->getSize().y < obj2->getPos().y + obj2->getSize().y)//y
		{
			if (obj1->getPos().z + obj1->getSize().z > obj2->getPos().z - obj2->getSize().z &&
				obj1->getPos().z - obj1->getSize().z < obj2->getPos().z + obj2->getSize().z)//z
			{
				return 1;
			}
		}
	}

	return 0;
}

bool Physics::checkCollision(CollideMesh* obj1, CollideMesh* obj2)
{
	return false;
}

bool Physics::checkCollision(glm::vec3* playerPos, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = i+1; j < size; j++)
		{
			if (playerPos[i] != glm::vec3(-1, -1, -1) && playerPos[j] != glm::vec3(-1, -1, -1))
			{
				//both are objects

				Geometry obj1 = Geometry(playerPos[i], this->size);
				Geometry obj2 = Geometry(playerPos[j], this->size);
				bool collide = checkCollision(&obj1, &obj2);
				return collide;
			}
		}
	}
	return false;
}

//TEMPORARY
void Physics::getBoatExtremes(glm::vec3 minVals, glm::vec3 maxVals)
{
	minExtremes = minVals;
	maxExtremes = maxVals;
	size = glm::vec3(abs(minExtremes.x) + abs(maxExtremes.x), abs(minExtremes.y) + abs(maxExtremes.y), abs(minExtremes.z) + abs(maxExtremes.z));

	size.x = size.x / 2;
	size.y = size.y / 2;
	size.z = size.z / 2;
}

Physics* CreatePhysics()
{
	return new Physics();
}