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

bool Physics::checkCollision(glm::vec3 pos, CollideMesh mesh)
{
	if (pos.x + size.x > mesh.getAABB().posX - mesh.getAABB().sizeX &&
		pos.x - size.x < mesh.getAABB().posX + mesh.getAABB().sizeX)//x
	{
		if (pos.y + size.y > mesh.getAABB().posY - mesh.getAABB().sizeY &&
			pos.y - size.y < mesh.getAABB().posY + mesh.getAABB().sizeY)//y
		{
			if (pos.z + size.z > mesh.getAABB().posZ - mesh.getAABB().sizeZ &&
				pos.z - size.z < mesh.getAABB().posZ + mesh.getAABB().sizeZ)//z
			{
				return 1;
			}
		}
	}

	return 0;
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
	Geometry bullet = Geometry(bulletPos, this->size);
	bool collide = checkCollision(&player, &bullet);

	return collide;
}


bool Physics::checkPlayerVWorldCollision(glm::vec3 playerPos)
{
	bool collides = false;
	for (int i = 0; i < worldBoxes.size(); i++)
	{
		if (checkCollision(playerPos, worldBoxes[i]))
			collides = true;
	}
	return collides;
}


void Physics::addGravity(glm::vec3 &vel, float dt)
{
	vel.y -= GRAVITY * dt;
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

void Physics::receiveWorldBoxes(std::vector<std::vector<float>> wBoxes)
{
	float xPos, yPos, zPos;
	float xSize, ySize, zSize;
	CollideMesh temp;
	for (int i = 0; i < wBoxes.size(); i++)
	{
		 xSize = abs(wBoxes[i][0] - wBoxes[i][1]);
		 ySize = abs(wBoxes[i][2] - wBoxes[i][3]);
		 zSize = abs(wBoxes[i][4] - wBoxes[i][5]);

		 xPos = (wBoxes[i][0] + wBoxes[i][1]) / 2;
		 yPos = (wBoxes[i][2] + wBoxes[i][3]) / 2;
		 zPos = (wBoxes[i][4] + wBoxes[i][5]) / 2;
		 
		 temp.addAABB(xPos, yPos, zPos, xSize, ySize, zSize);

		 worldBoxes.push_back(temp);
	}
}

Physics* CreatePhysics()
{
	return new Physics();
}