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

bool Physics::checkPlayerVPlayerCollision(glm::vec3 playerPos1, glm::vec3 playerPos2)
{

	Geometry obj1 = Geometry(playerPos1, this->size);
	Geometry obj2 = Geometry(playerPos2, this->size);
	bool collide = checkCollision(&obj1, &obj2);

	return collide;
}

bool Physics::checkPlayerVBulletCollision(glm::vec3 playerPos, glm::vec3 bulletPos)
{
	Geometry player = Geometry(playerPos, this->size);
	Geometry bullet = Geometry(playerPos, this->size);
	bool collide = checkCollision(&player, &bullet);

	return collide;
}

glm::vec3 Physics::addGravity(glm::vec3 pos, float dt)
{
	return glm::vec3(pos.x, pos.y - (GRAVITY * dt), pos.z);
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