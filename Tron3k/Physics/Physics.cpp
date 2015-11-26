#include "Physics.h"

Physics::Physics()
{

}

Physics::~Physics()
{

}

bool Physics::init()
{
	initBulletBox();
	return 1;
}

void Physics::initBulletBox()
{
	bulletBox.init();

	//TEMPORARY
	glm::vec3 size = glm::vec3(0.2f, 0.2f, 0.2f);

	bulletBox.setSize(size);
}

bool Physics::release()
{
	return 1;
}

bool Physics::checkAABBCollision(Geometry* obj1, Geometry* obj2)
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

bool Physics::checkAABBCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	if (mesh1.getAABB().posX + mesh1.getAABB().sizeX > mesh2.getAABB().posX - mesh2.getAABB().sizeX &&
		mesh1.getAABB().posX - mesh1.getAABB().sizeX < mesh2.getAABB().posX + mesh2.getAABB().sizeX)//x
	{
		if (mesh1.getAABB().posY + mesh1.getAABB().sizeY > mesh2.getAABB().posY - mesh2.getAABB().sizeY &&
			mesh1.getAABB().posY - mesh1.getAABB().sizeY < mesh2.getAABB().posY + mesh2.getAABB().sizeY)//y
		{
			if (mesh1.getAABB().posZ + mesh1.getAABB().sizeZ > mesh2.getAABB().posZ - mesh2.getAABB().sizeZ &&
				mesh1.getAABB().posZ - mesh1.getAABB().sizeZ < mesh2.getAABB().posZ + mesh2.getAABB().sizeZ)//z
			{
				return 1;
			}
		}
	}

	return 0;
}

bool Physics::checkOBBCollision(Geometry* obj1, Geometry* obj2)
{
	return 1;
}

bool Physics::checkOBBCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return 1;
}

bool Physics::checkPlayerVPlayerCollision(glm::vec3 playerPos1, glm::vec3 playerPos2)
{
	playerBox.setPos(playerPos1);
	CollideMesh p2;
	p2.init();
	p2.setPos(playerPos2);
	p2.setSize(playerBox.getSize());


	bool collide = checkAABBCollision(playerBox, p2);

	//if (collide)
		//collide = checkOBBCollision(&obj1, &obj2);
	return collide;
}

bool Physics::checkPlayerVBulletCollision(glm::vec3 playerPos, glm::vec3 bulletPos)
{
	playerBox.setPos(playerPos);
	bulletBox.setPos(bulletPos);
	bool collide = checkAABBCollision(playerBox, bulletBox);

	//if (collide)
		//collide = checkOBBCollision(&player, &bullet);
	return collide;
}


bool Physics::checkPlayerVWorldCollision(glm::vec3 playerPos)
{
	bool collides = false;
	playerBox.setPos(playerPos);

	for (int i = 0; i < worldBoxes.size(); i++)
	{
		if (checkAABBCollision(playerBox, worldBoxes[i]))
			//if (checkOBBCollision(playerPos, worldBoxes[i]))
				collides = true;
	}
	return collides;
}


void Physics::addGravity(glm::vec3 &vel, float dt)
{
	vel.y -= GRAVITY * dt;
}

void Physics::receivePlayerBox(std::vector<float> pBox)
{
	float xPos, yPos, zPos;
	float xSize, ySize, zSize;
	playerBox.init();

	xSize = abs(pBox[0] - pBox[1]) / 2;
	ySize = abs(pBox[2] - pBox[3]) / 2;
	zSize = abs(pBox[4] - pBox[5]) / 2;

	xPos = (pBox[0] + pBox[1]) / 2;
	yPos = (pBox[2] + pBox[3]) / 2;
	zPos = (pBox[4] + pBox[5]) / 2;

	playerBox.setAABB(xPos, yPos, zPos, xSize, ySize, zSize);
}

void Physics::receiveWorldBoxes(std::vector<std::vector<float>> wBoxes)
{
	float xPos, yPos, zPos;
	float xSize, ySize, zSize;
	CollideMesh temp;
	temp.init();
	for (int i = 0; i < wBoxes.size(); i++)
	{
		xSize = abs(wBoxes[i][0] - wBoxes[i][1]) / 2;
		ySize = abs(wBoxes[i][2] - wBoxes[i][3]) / 2;
		zSize = abs(wBoxes[i][4] - wBoxes[i][5]) / 2;

		xPos = (wBoxes[i][0] + wBoxes[i][1]) / 2;
		yPos = (wBoxes[i][2] + wBoxes[i][3]) / 2;
		zPos = (wBoxes[i][4] + wBoxes[i][5]) / 2;

		temp.setAABB(xPos, yPos, zPos, xSize, ySize, zSize);

		worldBoxes.push_back(temp);
	}
}

Physics* CreatePhysics()
{
	return new Physics();
}