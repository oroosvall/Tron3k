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

Physics* CreatePhysics()
{
	return new Physics();
}