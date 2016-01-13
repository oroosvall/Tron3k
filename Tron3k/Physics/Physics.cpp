#include "Physics.h"

//#include <vld.h>

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
	bulletBox.setAABB(glm::vec3(0, 0, 0), size, -size);

	//bulletBox.setSize(size);
}

bool Physics::release()
{
	delete this;
	return 1;
}

//--------AABB Collisions--------//
glm::vec3 Physics::checkAABBvAABBCollision(Geometry* obj1, Geometry* obj2)
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
				return glm::vec3(1, 1, 1);
			}
		}
	}

	return glm::vec3(0, 0, 0);
}

bool Physics::checkAABBvAABBCollision(CollideMesh mesh1, CollideMesh mesh2, bool moveSecond)
{
	AABB* aabb1 = mesh1.getAABB();
	AABB* aabb2 = mesh2.getAABB();

	glm::vec3 pos1 = glm::vec3(aabb1->pos);
	glm::vec3 pos2 = glm::vec3(aabb2->pos);

	glm::vec3 max1 = glm::vec3(aabb1->max);
	glm::vec3 max2 = glm::vec3(aabb2->max);

	glm::vec3 min1 = glm::vec3(aabb1->min);
	glm::vec3 min2 = glm::vec3(aabb2->min);

	if (moveSecond)
	{
		//max1 = max1 - pos1;
		max2 = max2 - pos2;

		//min1 = min1 - pos1;
		min2 = min2 - pos2;
	}


	if (pos1.x + max1.x + 0.01f > pos2.x + min2.x - 0.01f &&
		pos1.x + min1.x - 0.01f < pos2.x + max2.x + 0.01f)//x
	{
		if (pos1.y + max1.y + 0.01f > pos2.y + min2.y - 0.01f &&
			pos1.y + min1.y - 0.01f < pos2.y + max2.y + 0.01f)//y
		{
			if (pos1.z + max1.z + 0.01f > pos2.z + min2.z - 0.01f &&
				pos1.z + min1.z - 0.01f < pos2.z + max2.z + 0.01f)//z
			{
				return true;
			}
		}
	}

	return false;
}

glm::vec3 Physics::checkAABBvCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	Cylinder cylinder;
	AABB* aabb;

	cylinder = mesh1.getCylinder();
	aabb = mesh2.getAABB();

	glm::vec3 max = glm::vec3(aabb->max - aabb->pos);
	glm::vec3 min = glm::vec3(aabb->min - aabb->pos);
	glm::vec3 pos = glm::vec3(aabb->pos);


	if (cylinder.pos.y + cylinder.height > pos.y - min.y &&
		cylinder.pos.y - cylinder.height < pos.y + max.y)
	{
		//Collides in Y

		//http://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection

		glm::vec2 dist = glm::vec2(abs(cylinder.pos.x - pos.x), abs(cylinder.pos.z - pos.z));

		if (dist.x > max.x + cylinder.radius)
			return glm::vec3(0, 0, 0);
		if (dist.y > max.z + cylinder.radius)
			return glm::vec3(0, 0, 0);

		if (dist.x <= max.x)
			return glm::vec3(1, 1, 1);
		if (dist.y <= max.z)
			return glm::vec3(1, 1, 1);

		float cDist = ((dist.x - min.x) * (dist.x - min.x)) + ((dist.y - min.z) * (dist.y - min.z));

		if (cDist <= cylinder.radius * cylinder.radius)
			return glm::vec3(1, 1, 1);// getCollisionNormal(mesh1, mesh2);
		return glm::vec3(0, 0, 0);

	}

	return glm::vec3(0, 0, 0);
	/*if (mesh1.getAABB().posX + mesh1.getAABB().sizeX > mesh2.getAABB().posX - mesh2.getAABB().sizeX &&
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

	return 0;*/
}

glm::vec3 Physics::checkAABBvAngledCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::checkAABBvSphereCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return glm::vec3(1, 1, 1);
}
//--------------//--------------//

//--------OBB Collisions--------//
glm::vec3 Physics::checkOBBvOBBCollision(Geometry* obj1, Geometry* obj2)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::checkOBBvOBBCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::checkOBBvCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::checkOBBvAngledCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::checkOBBvSphereCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return glm::vec3(1, 1, 1);
}
//--------------//--------------//

//-----Cylinder Collisions------//
glm::vec3 Physics::checkCylindervCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::checkCylindervSphereCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::checkAngledCylindervSphereCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	return glm::vec3(1, 1, 1);
}
//--------------//--------------//

float Physics::checkLinevPlaneCollision(glm::vec3 l1, glm::vec3 l2, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 p = glm::vec3(0, 0, 0);

	float x = 1;
	glm::vec3 l = (l2 - l1);
	glm::vec3 l3 = l;
	normalize(l);

	p = normalize(glm::cross((p1 - p2), (p3 - p2)));

	float d = -((p2.x * p.x) + (p2.y * p.y) + (p2.z + p.z));

	//the equation for the plane is now p + d

	if (glm::dot(l, p) == 0)
		return -1;

	x = (glm::dot(p2-l1, p) / (glm::dot(l3, p)));

	if (x >= 0 && x <= 1)
		return x;
	return -1;

}

//------Normal Calculators------//
std::vector<glm::vec3> Physics::getCollisionNormal(AABB* aabb1, AABB* aabb2)
{
	glm::vec3 l1 = glm::vec3(aabb1->pos);
	glm::vec3 l2 = glm::vec3(aabb2->pos);

	CollideMesh temp;
	temp.init();

	std::vector<glm::vec3> norms;

	temp.setAABB(glm::vec3(aabb2->pos), glm::vec3(aabb2->max), glm::vec3(aabb2->min));

	//P2 should be center point, just in case
	glm::vec3 p1, p2, p3;
	p1 = p2 = p3 = glm::vec3(0, 0, 0);

	float collides1 = -1;
	float collides2 = -1;

	bool collideX, collideY, collideZ;
	collideX = collideY = collideZ = false;

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[1]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);



	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideZ)
	{
		//Front plane collision
		collideZ = true;
		norms.push_back(glm::vec3(0, 0, collides1));
	}

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[6]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);



	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideZ)
	{
		//Back plane collision
		collideZ = true;
		norms.push_back(glm::vec3(0, 0, -collides1));
	}

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[7]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);


	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideY)
	{
		//Top plane collision
		collideY = true;
		norms.push_back(glm::vec3(0, collides1, 0));
	}

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[0]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);


	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideY)
	{
		//Bot plane collision
		collideY = true;
		norms.push_back(glm::vec3(0, -collides1, 0));
	}

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[3]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);



	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideX)
	{
		//Right plane collision
		collideX = true;
		norms.push_back(glm::vec3(collides1, 0, 0));
	}

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[4]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);



	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideX)
	{
		//Left plane collision
		collideX = true;
 		norms.push_back(glm::vec3(-collides1, 0, 0));
	}

	return norms;
}

std::vector<glm::vec4> Physics::sphereVSobbNorms(glm::vec3 pos, float rad, OBB* obb)
{
	std::vector<glm::vec4> norms;
	//P2 should be center point, just in case
	//glm::vec3 p1, p2, p3;
	//p1 = p2 = p3 = glm::vec3(0, 0, 0);
	//
	//float collides1 = -1;
	//float collides2 = -1;
	//
	//bool collideX, collideY, collideZ;
	//collideX = collideY = collideZ = false;
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	//
	//collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	//
	//collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//
	//
	//if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideZ)
	//{
	//	//Front plane collision
	//	collideZ = true;
	//	norms.push_back(glm::vec3(0, 0, collides1));
	//}
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	//
	//collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	//
	//collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//
	//
	//if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideZ)
	//{
	//	//Back plane collision
	//	collideZ = true;
	//	norms.push_back(glm::vec3(0, 0, -collides1));
	//}
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	//
	//collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	//
	//collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//
	//if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideY)
	//{
	//	//Top plane collision
	//	collideY = true;
	//	norms.push_back(glm::vec3(0, collides1, 0));
	//}
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	//
	//collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	//
	//collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//
	//if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideY)
	//{
	//	//Bot plane collision
	//	collideY = true;
	//	norms.push_back(glm::vec3(0, -collides1, 0));
	//}
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	//
	//collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	//
	//collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//
	//
	//if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideX)
	//{
	//	//Right plane collision
	//	collideX = true;
	//	norms.push_back(glm::vec3(collides1, 0, 0));
	//}
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	//
	//collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//p1 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	//p2 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	//p3 = glm::vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	//
	//collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);
	//
	//
	//
	//if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideX)
	//{
	//	//Left plane collision
	//	collideX = true;
	//	norms.push_back(glm::vec3(-collides1, 0, 0));
	//}

	return norms;
}

glm::vec3 Physics::getCollisionNormal(Cylinder cylinder, AABB aabb)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::getCollisionNormal(AngledCylinder cylinder, AABB aabb)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::getCollisionNormal(Sphere sphere, AABB aabb)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::getCollisionNormal(OBB obb1, OBB obb2)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::getCollisionNormal(Cylinder cylinder, OBB obb)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::getCollisionNormal(AngledCylinder cylinder, OBB obb)
{
	return glm::vec3(1, 1, 1);
}

glm::vec3 Physics::getCollisionNormal(Sphere sphere, OBB obb)
{
	return glm::vec3(1, 1, 1);
}
//--------------//--------------//

glm::vec3 Physics::checkPlayerVPlayerCollision(glm::vec3 playerPos1, glm::vec3 playerPos2)
{
	playerBox.setPos(playerPos1);
	CollideMesh p2;
	p2.init();
	p2.setPos(playerPos2);
	//p2.setSize(playerBox.getSize());


	bool ret = checkAABBvAABBCollision(playerBox, p2, false);
	if (ret)
		return glm::vec3(1, 1, 1);
	return glm::vec3(0, 0, 0);
}

glm::vec3 Physics::checkPlayerVBulletCollision(glm::vec3 playerPos, glm::vec3 bulletPos)
{
	playerBox.setPos(playerPos);
	bulletBox.setPos(bulletPos);
	glm::vec3 collide = glm::vec3(0, 0, 0);// checkAABBCollision(playerBox, bulletBox);

	if (checkAABBvAABBCollision(playerBox, bulletBox, false))
		return normalize( playerPos - bulletPos);

	return collide;
}

std::vector<glm::vec4> Physics::checkPlayerVWorldCollision(glm::vec3 playerPos, float rad)
{
	glm::vec4 temp = glm::vec4(playerPos, 1.0f);
	AABB box;
	box.max = temp + glm::vec4(rad, rad, rad, 0);
	box.min = temp - glm::vec4(rad, rad, rad, 0);
	playerBox.setAABB(box);

	std::vector<glm::vec4> cNorms;
	std::vector<glm::vec4> t;
	glm::vec3 collisionNormal = glm::vec3(0, 0, 0);

	for (unsigned int i = 0; i < worldBoxes.size(); i++)
	{
		for (unsigned int j = 0; j < worldBoxes[i].size(); j++)
		{
			if (checkAABBvAABBCollision(playerBox, worldBoxes[i][j]))
			{
				//for each obb contained in that abb
				int size = worldBoxes[i][j].boundingBox.ObbBoxes.size();
				for (int n = 0; n < size; n++)
				{
					t = sphereVSobbNorms(playerPos, rad, &worldBoxes[i][j].boundingBox.ObbBoxes[n]);

					cNorms.reserve(cNorms.size() + t.size());
					std::move(t.begin(), t.end(), std::inserter(cNorms, cNorms.end()));
					t.clear();
				}
			}
		}
	}

	return cNorms;
}

glm::vec3 Physics::checkBulletVWorldCollision(glm::vec3 bulletPos)
{
	glm::vec3 collides = glm::vec3(0, 0, 0);
	bulletBox.setPos(bulletPos);

	for (unsigned int i = 0; i < worldBoxes.size(); i++)
	{
		for (unsigned int j = 0; j < worldBoxes[i].size(); j++)
		{

			if (checkAABBvAABBCollision(bulletBox, worldBoxes[i][j]))
			{
				collides = glm::vec3(1, 1, 1);
			}
		}
	}
	return collides;
}

void Physics::addGravity(glm::vec3 &vel, float dt)
{
	vel.y -= GRAVITY * dt;
}

glm::vec3 Physics::normalize(glm::vec3 &vec3)
{
	float len = abs(vec3.x) + abs(vec3.y) + abs(vec3.z);

	if (len > FLT_EPSILON)
	{
		vec3.x = vec3.x / len;
		vec3.y = vec3.y / len;
		vec3.z = vec3.z / len;
	}

	return vec3;

}

void Physics::receiveChunkBoxes(int chunkID, void* cBoxes)
{
	std::vector<AABB>* cBox = (std::vector<AABB>*)cBoxes;
	std::vector<AABB> b = std::vector<AABB>(*cBox);

	storeChunkBox(chunkID, b);
}

void Physics::storeChunkBox(int chunkID, std::vector<AABB> cBoxes)
{
	//chunkID isn't used, but we send it here anyway, JUST IN CASE

	//creates a vector, so we know information is copied
	std::vector<CollideMesh> cMeshes;
	CollideMesh temp;

	for (unsigned int i = 0; i < cBoxes.size(); i++)
	{
		temp = CollideMesh();
		//stores all the info in a tempmesh
		temp.init();
		temp.setAABB(cBoxes[i]);

		//adds it to our vector
		cMeshes.push_back(temp);
	}

	//Adds the vector to the WorldBox
	//This vector is all the collisionboxes in a chunk
	//so Chunk[0] will have it's collisionBoxes in worldBoxes[0]
	worldBoxes.push_back(cMeshes);

}

void Physics::receivePlayerBox(std::vector<float> pBox)
{
	float xPos, yPos, zPos;
	float xSize, ySize, zSize;
	playerBox.init();

	xSize = (abs(pBox[0]) + abs(pBox[1])) / 2;
	ySize = (abs(pBox[2]) + abs(pBox[3])) / 2;
	zSize = (abs(pBox[4]) + abs(pBox[5])) / 2;

	xPos = (pBox[0] + pBox[1]) / 2;
	yPos = (pBox[2] + pBox[3]) / 2;
	zPos = (pBox[4] + pBox[5]) / 2;

	playerBox.setAABB(glm::vec3(xPos, yPos, zPos), glm::vec3(pBox[0], pBox[2], pBox[4]), glm::vec3(pBox[1], pBox[3], pBox[5]));
}

void Physics::receiveWorldBoxes(std::vector<std::vector<float>> wBoxes)
{
	float xPos, yPos, zPos;
	float xSize, ySize, zSize;
	CollideMesh temp;
	temp.init();
	for (unsigned int i = 0; i < wBoxes.size(); i++)
	{
		xSize = abs(wBoxes[i][0] - wBoxes[i][1]) / 2;
		ySize = abs(wBoxes[i][2] - wBoxes[i][3]) / 2;
		zSize = abs(wBoxes[i][4] - wBoxes[i][5]) / 2;

		xPos = (wBoxes[i][0] + wBoxes[i][1]) / 2;
		yPos = (wBoxes[i][2] + wBoxes[i][3]) / 2;
		zPos = (wBoxes[i][4] + wBoxes[i][5]) / 2;

		temp.setAABB(glm::vec3(xPos, yPos, zPos), glm::vec3(xSize, ySize, zSize), glm::vec3(-xSize, -ySize, -zSize));

		worldBoxes[0].push_back(temp);
	}
}

Physics* CreatePhysics()
{
	return new Physics();
}