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
	bulletBox.init();
	vec3 size = vec3(0.2f, 0.2f, 0.2f);
	bulletBox.setAABB(vec3(0, 0, 0), size, -size);
	bulletBox.setSphere(vec3(0, 0, 0), 0.4f);

	//bulletBox.setSize(size);
}

bool Physics::release()
{
	delete this;
	return 1;
}

//--------AABB Collisions--------//
vec3 Physics::checkAABBvAABBCollision(Geometry* obj1, Geometry* obj2)
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
				return vec3(1, 1, 1);
			}
		}
	}

	return vec3(0, 0, 0);
}

bool Physics::checkAABBvAABBCollision(AABB* mesh1, AABB* mesh2)
{
	if (mesh1->max.x + FLT_EPSILON >= mesh2->min.x - FLT_EPSILON && mesh1->min.x - FLT_EPSILON <= mesh2->max.x + FLT_EPSILON)//x
		if (mesh1->max.y + FLT_EPSILON >= mesh2->min.y - FLT_EPSILON && mesh1->min.y - FLT_EPSILON <= mesh2->max.y + FLT_EPSILON)//y
			if (mesh1->max.z + FLT_EPSILON >= mesh2->min.z - FLT_EPSILON && mesh1->min.z - FLT_EPSILON <= mesh2->max.z + FLT_EPSILON)//y
				return true;

	return false;
}

bool Physics::checkPlayerVCapCollision(AABB* player, AABBCapPoint capPoint)
{
	if (player->max.x + FLT_EPSILON >= capPoint.min.x - FLT_EPSILON && player->min.x - FLT_EPSILON <= capPoint.max.x + FLT_EPSILON)//x
		if (player->max.y + FLT_EPSILON >= capPoint.min.y - FLT_EPSILON && player->min.y - FLT_EPSILON <= capPoint.max.y + FLT_EPSILON)//y
			if (player->max.z + FLT_EPSILON >= capPoint.min.z - FLT_EPSILON && player->min.z - FLT_EPSILON <= capPoint.max.z + FLT_EPSILON)//y
			{
				//we collided with the outer box
				bool innerCollide = false;
				for (int i = 0; i < capPoint.aabbs.size() && !innerCollide; i++)
				{
					if (player->max.x + FLT_EPSILON >= capPoint.aabbs[i].min.x - FLT_EPSILON && player->min.x - FLT_EPSILON <= capPoint.aabbs[i].max.x + FLT_EPSILON)//x
						if (player->max.y + FLT_EPSILON >= capPoint.aabbs[i].min.y - FLT_EPSILON && player->min.y - FLT_EPSILON <= capPoint.aabbs[i].max.y + FLT_EPSILON)//y
							if (player->max.z + FLT_EPSILON >= capPoint.aabbs[i].min.z - FLT_EPSILON && player->min.z - FLT_EPSILON <= capPoint.aabbs[i].max.z + FLT_EPSILON)//y
								return true;

				}
			}
	return false;
}

vec3 Physics::checkAABBvCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	Cylinder cylinder;
	AABB* aabb;

	cylinder = mesh1.getCylinder();
	aabb = mesh2.getAABB();

	vec3 max = vec3(aabb->max - aabb->pos);
	vec3 min = vec3(aabb->min - aabb->pos);
	vec3 pos = vec3(aabb->pos);


	if (cylinder.pos.y + cylinder.height > pos.y - min.y &&
		cylinder.pos.y - cylinder.height < pos.y + max.y)
	{
		//Collides in Y

		//http://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection

		vec2 dist = vec2(abs(cylinder.pos.x - pos.x), abs(cylinder.pos.z - pos.z));

		if (dist.x > max.x + cylinder.radius)
			return vec3(0, 0, 0);
		if (dist.y > max.z + cylinder.radius)
			return vec3(0, 0, 0);

		if (dist.x <= max.x)
			return vec3(1, 1, 1);
		if (dist.y <= max.z)
			return vec3(1, 1, 1);

		float cDist = ((dist.x - min.x) * (dist.x - min.x)) + ((dist.y - min.z) * (dist.y - min.z));

		if (cDist <= cylinder.radius * cylinder.radius)
			return vec3(1, 1, 1);// getCollisionNormal(mesh1, mesh2);
		return vec3(0, 0, 0);

	}

	return vec3(0, 0, 0);
}

vec3 Physics::checkAABBvAngledCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}

vec3 Physics::checkAABBvSphereCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}
//--------------//--------------//

//--------OBB Collisions--------//
vec3 Physics::checkOBBvOBBCollision(Geometry* obj1, Geometry* obj2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}

vec3 Physics::checkOBBvOBBCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}

vec3 Physics::checkOBBvCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}

vec3 Physics::checkOBBvAngledCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}
//--------------//--------------//

//-----Cylinder Collisions------//
vec3 Physics::checkCylindervCylinderCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}

vec3 Physics::checkCylindervSphereCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}

vec3 Physics::checkAngledCylindervSphereCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}
//--------------//--------------//

//-------Sphere Collision-------//
std::vector<glm::vec4> Physics::checkSpherevSphereCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	std::vector<glm::vec4> collided;

	glm::vec3 dist = mesh1.getSphere().pos - mesh2.getSphere().pos;
	float radius = mesh1.getSphere().radius + mesh2.getSphere().radius;

	if (length(dist) <= radius)
	{
		//collision
		collided.push_back(vec4(dist, radius - length(dist)));

	}
	return collided;
}

std::vector<glm::vec4> Physics::checkSpherevSpheretdCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	std::vector<glm::vec4> collided;

	glm::vec3 dist = mesh1.getSphere().pos - mesh2.getSphere().pos;
	float radius = mesh1.getSphere().radius + mesh2.getSphere().radius;
	float minRad = mesh1.getSphere().radius - 3.0f + mesh2.getSphere().radius;

	if (length(dist) <= radius && length(dist) >= minRad)
	{
		//collision
		vec3 d = normalize(dist);

		//if(inside)
		if (abs(length(dist) - minRad) < abs(length(dist) - radius))
		{
			collided.push_back(vec4(normalize(dist), -abs(minRad - length(dist)))); //works for when we're inside sphere
		}
		else
		{
			/*works if we outside sphere*/
			//dist = dist - d * mesh1.getSphere().radius;
			collided.push_back(vec4(normalize(dist), abs(radius - length(dist))));
		}
	}
	return collided;
}

std::vector<glm::vec4> Physics::checkSpherevOBBlwCollision(CollideMesh mesh1, CollideMesh mesh2) //mesh 1 = Sphere, mesh2 = obb
{
	std::vector<vec4> cNorms;
	vec4 t = glm::vec4(0);
	vec3 collisionNormal = vec3(0, 0, 0);
	mesh2.boundingBox.ObbBoxes[0].setPlanes();



	if (checkAABBvAABBCollision(mesh1.getAABB(), mesh2.getAABB()))
	{
		//printf("%d %d \n", i, j); // test for abbs so they register
		float rad = mesh1.getSphere().radius;
		//for each obb contained in that abb
		int size = mesh2.boundingBox.ObbBoxes.size();
		for (int n = 0; n < size; n++)
		{
			t = getSpherevOBBlwNorms(mesh1.getSphere().pos, mesh1.getSphere().radius, &mesh2.boundingBox.ObbBoxes[n]);
			t.w = rad - t.w; //penetration depth instead of collision distance 
			if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= mesh1.getSphere().radius + FLT_EPSILON)
			{
				t = vec4(normalize(vec3(t)), t.w);
				cNorms.push_back(t);
			}
		}
	}

	return cNorms;
}
//--------------//--------------//

float Physics::checkLinevPlaneCollision(vec3 l1, vec3 l2, vec3 p1, vec3 p2, vec3 p3)
{
	vec3 p = vec3(0, 0, 0);

	float x = 1;
	vec3 l = (l2 - l1);
	vec3 l3 = l;
	normalize(l);

	p = normalize(cross((p1 - p2), (p3 - p2)));

	float d = -((p2.x * p.x) + (p2.y * p.y) + (p2.z + p.z));

	//the equation for the plane is now p + d

	if (dot(l, p) == 0)
		return -1;

	x = (dot(p2 - l1, p) / (dot(l3, p)));

	if (x >= 0 && x <= 1)
		return x;
	return -1;

}

//------Normal Calculators------//
std::vector<vec3> Physics::getCollisionNormal(AABB* aabb1, AABB* aabb2)
{
	vec3 l1 = vec3(aabb1->pos);
	vec3 l2 = vec3(aabb2->pos);

	CollideMesh temp;
	temp.init();

	std::vector<vec3> norms;

	temp.setAABB(vec3(aabb2->pos), vec3(aabb2->max), vec3(aabb2->min));

	//P2 should be center point, just in case
	vec3 p1, p2, p3;
	p1 = p2 = p3 = vec3(0, 0, 0);

	float collides1 = -1;
	float collides2 = -1;

	bool collideX, collideY, collideZ;
	collideX = collideY = collideZ = false;

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[1]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[2]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);



	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideZ)
	{
		//Front plane collision
		collideZ = true;
		norms.push_back(vec3(0, 0, collides1));
	}

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[5]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[6]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);



	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideZ)
	{
		//Back plane collision
		collideZ = true;
		norms.push_back(vec3(0, 0, -collides1));
	}

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[7]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[2]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);


	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideY)
	{
		//Top plane collision
		collideY = true;
		norms.push_back(vec3(0, collides1, 0));
	}

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[0]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[5]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);


	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideY)
	{
		//Bot plane collision
		collideY = true;
		norms.push_back(vec3(0, -collides1, 0));
	}

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[3]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[7]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[5]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[5]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[1]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[3]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);



	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideX)
	{
		//Right plane collision
		collideX = true;
		norms.push_back(vec3(collides1, 0, 0));
	}

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[2]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[6]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[4]);

	collides1 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);

	p1 = vec3(temp.boundingBox.ObbBoxes[0].corners[4]);
	p2 = vec3(temp.boundingBox.ObbBoxes[0].corners[0]);
	p3 = vec3(temp.boundingBox.ObbBoxes[0].corners[2]);

	collides2 = checkLinevPlaneCollision(l1, l2, p1, p2, p3);



	if ((collides1 >= 0.0f || collides2 >= 0.0f) && !collideX)
	{
		//Left plane collision
		collideX = true;
		norms.push_back(vec3(-collides1, 0, 0));
	}

	return norms;
}

vec4 Physics::getSpherevOBBNorms(vec3 pos, float rad, OBB* obb)
{
	vec4 t;
	vec4 closest;
	closest.w = FLT_MAX;

	//test vs all planes
	for (int n = 0; n < 6; n++)
	{
		// send inverse plane normal as dir
		// t.w will be distance to the wall -1 if no wall was close enough
		t = obb->planes[n].intersects(pos, -obb->planes[n].n, rad);

		// if a valid intersection found
		//plane intersection will always be closer than all other intersections on the obb
		if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON)
		{
			return t;
		}
	}

	//if no plane intersections found
	//test all lines  

	for (int n = 0; n < 12; n++)
	{
		t = obb->lines[n].sphere_intersects(pos, rad);
		if (obb->lines[n].line.y < -FLT_EPSILON || obb->lines[n].line.y > FLT_EPSILON)
			if (t.w >= 0 - FLT_EPSILON)
				if (t.w < closest.w) // a new closest dist was found
					closest = t;
	}

	//if we found a line intersection it will always be closer
	//than all the corner intersections
	if (closest.w < FLT_MAX)
		return closest;

	vec3 test;
	float test_len;
	//each corner
	for (int n = 0; n < 8; n++)
	{
		test = pos - obb->corners[n];
		test_len = length(test);

		if (test_len <= closest.w)
		{
			closest.x = test.x; closest.y = test.y; closest.z = test.z;
			closest.w = test_len;
		}
	}

	// if the closest one if further than sphere rad = no intersection
	if (closest.w + FLT_EPSILON > rad - FLT_EPSILON)
		closest.w = -1;

	return closest;
}

vec4 Physics::getSpherevOBBlwNorms(vec3 pos, float rad, OBB* obb)
{
	vec4 t;
	vec4 closest;
	closest.w = FLT_MAX;

	//test vs all planes
	for (int n = 0; n < 6; n++)
	{
		// send inverse plane normal as dir
		// t.w will be distance to the wall -1 if no wall was close enough
		t = obb->planes[n].intersects(pos, -obb->planes[n].n, rad);

		// if a valid intersection found
		//plane intersection will always be closer than all other intersections on the obb
		if (t.w > 0)
		{
			return t;
		}
	}



	// if the closest one if further than sphere rad = no intersection
	if (closest.w + FLT_EPSILON > rad - FLT_EPSILON)
		closest.w = -1;

	return closest;
}

vec3 Physics::getCollisionNormal(Cylinder cylinder, AABB aabb)
{
	glm::vec3 normal = glm::vec3(0, 0, 0);
	return normal;
}

vec3 Physics::getCollisionNormal(AngledCylinder cylinder, AABB aabb)
{
	glm::vec3 normal = glm::vec3(0, 0, 0);
	return normal;
}

vec3 Physics::getCollisionNormal(Sphere sphere, AABB aabb)
{
	glm::vec3 normal = glm::vec3(0, 0, 0);
	return normal;
}

vec3 Physics::getCollisionNormal(OBB obb1, OBB obb2)
{
	glm::vec3 normal = glm::vec3(0, 0, 0);
	return normal;
}

vec3 Physics::getCollisionNormal(Cylinder cylinder, OBB obb)
{
	glm::vec3 normal = glm::vec3(0, 0, 0);
	return normal;
}

vec3 Physics::getCollisionNormal(AngledCylinder cylinder, OBB obb)
{
	glm::vec3 normal = glm::vec3(0, 0, 0);
	return normal;
}

vec3 Physics::getCollisionNormal(Sphere sphere, OBB obb)
{
	glm::vec3 normal = glm::vec3(0, 0, 0);
	return normal;
}
//--------------//--------------//

vec3 Physics::checkPlayerVPlayerCollision(vec3 playerPos1, vec3 playerPos2)
{
	playerBox.setPos(playerPos1);
	CollideMesh p2;
	p2.init();
	p2.setPos(playerPos2);
	//p2.setSize(playerBox.getSize());


	bool ret = checkAABBvAABBCollision(&playerBox.boundingBox, &p2.boundingBox);
	if (ret)
		return vec3(1, 1, 1);
	return vec3(0, 0, 0);
}

vec3 Physics::checkPlayerVBulletCollision(vec3 playerPos, vec3 bulletPos, vec3 size)
{
	playerBox.setPos(playerPos);
	bulletBox.setPos(bulletPos);
	float prad = playerBox.getSphere().radius;
	AABB box;
	box.max = playerPos + size;
	box.min = playerPos - size;
	playerBox.setAABB(box);

	//TEMPORARY

	float brad = bulletBox.getSphere().radius;
	box.max = bulletPos + vec3(0.2, 0.2, 0.2);
	box.min = bulletPos - vec3(0.2, 0.2, 0.2);
	bulletBox.setAABB(box);


	vec3 collide = vec3(0, 0, 0);// checkAABBCollision(playerBox, bulletBox);

	if (checkAABBvAABBCollision(&playerBox.boundingBox, &bulletBox.boundingBox))
		return normalize(playerPos - bulletPos);

	return collide;
}

std::vector<vec4> Physics::PlayerVWorldCollision(vec3 playerPos)
{
	AABB box;
	float rad = playerBox.getSphere().radius;
	box.max = playerPos + vec3(rad, rad, rad);
	box.min = playerPos - vec3(rad, rad, rad);
	playerBox.setAABB(box);

	std::vector<vec4> cNorms;
	vec4 t;
	vec3 collisionNormal = vec3(0, 0, 0);

	//each chunk
	for (unsigned int i = 0; i < worldBoxes.size(); i++)
	{
		//culling player intersection tests vs room aabbs
		if (i > 0)
		{
			bool passed = false;
			if (box.max.x > roomBoxes[i - 1].min.x && box.min.x < roomBoxes[i - 1].max.x)//x
				if (box.max.y > roomBoxes[i - 1].min.y && box.min.y < roomBoxes[i - 1].max.y)//y
					if (box.max.z > roomBoxes[i - 1].min.z && box.min.z < roomBoxes[i - 1].max.z)//y
						passed = true;
		
			if (passed == false)
				continue;	
		}
		//printf("inside room %d \n", i);

		//each abb
		for (unsigned int j = 0; j < worldBoxes[i].size(); j++)
		{
			bool contin = false;

			if (checkAABBvAABBCollision(&playerBox.boundingBox, &worldBoxes[i][j].boundingBox))
			{
				//printf("%d %d \n", i, j); // test for abbs so they register

				//for each obb contained in that abb
				int size = worldBoxes[i][j].boundingBox.ObbBoxes.size();

				for (int n = 0; n < size; n++)
				{
					t = getSpherevOBBNorms(playerPos, rad, &worldBoxes[i][j].boundingBox.ObbBoxes[n]);
					t.w = rad - t.w; //penetration depth instead of collision distance 
					if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
					{
						t = vec4(normalize(vec3(t)), t.w);
						cNorms.push_back(t);
					}
				}
			}
		}
	}

	return cNorms;
}

void* Physics::checkBulletvWorldInternal(AABB bulletBox, float rad, int index)
{
	std::vector<vec4> cNorms;

	//each chunk
	vec4 t = vec4(0);
	for (unsigned int i = 0; i < worldBoxes.size(); i++)
	{
		//each abb
		for (unsigned int j = 0; j < worldBoxes[i].size(); j++)
		{
			bool contin = false;

			//do or do not, there is a try again at half the position

			if (checkAABBvAABBCollision(&bulletBox, &worldBoxes[i][j].boundingBox))
			{
				//printf("%d %d \n", i, j); // test for abbs so they register

				//for each obb contained in that abb
				int size = worldBoxes[i][j].boundingBox.ObbBoxes.size();
				for (int n = 0; n < size; n++)
				{
					t = getSpherevOBBNorms(bulletBox.pos, rad, &worldBoxes[i][j].boundingBox.ObbBoxes[n]);
					t.w = rad - t.w; //penetration depth instead of collision distance 
					if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
					{
						t = vec4(normalize(vec3(t)), t.w);
						t.w = t.w * (4 - i);//gets the pendepth based on where in the dt we are
						bulletNormal[index] = t;
						return (void*)&t;
					}
				}
			}
		}
	}
	return (void*)&t;
}

vec4 Physics::BulletVWorldCollision(vec3 bulletPos, vec3 bulletVel, vec3 bulletDir, float dt)
{
	for (int i = 0; i < 4; i++)
	{
		bulletNormal[i] = (vec4(0));
	}
	AABB box;
	vec3 bPos = bulletPos - (bulletVel * bulletDir * dt);
	float rad = bulletBox.getSphere().radius;
	box.max = bPos + vec3(rad, rad, rad);
	box.min = bPos - vec3(rad, rad, rad);
	bulletBox.setAABB(box);

	std::vector<vec4> cNorms;
	vec4 t;
	vec3 collisionNormal = vec3(0, 0, 0);

	float dtbyI = 0.0f;
	vec3 deltaDir = vec3(0);
	vec3 dirTimesVel = vec3(0);
	dtbyI = dt / 4;

	//bPos += bulletVel * bulletDir * (dt / (float)i);
	deltaDir = bulletDir * dtbyI;
	dirTimesVel = bulletVel * deltaDir;

	//std::thread bthreads[4];
	AABB roomBox;
	for (int k = 0; k < 4; k++)
	{


		bPos += dirTimesVel;
		box.max = bPos + vec3(rad, rad, rad);
		box.min = bPos - vec3(rad, rad, rad);
		bulletBox.setAABB(box);
		//each chunk
		vec4 t = vec4(0);
		for (unsigned int i = 0; i < worldBoxes.size(); i++)
		{
			//each abb
			
			roomBox.pos = roomBoxes[i].pos;
			roomBox.max = roomBoxes[i].max;
			roomBox.min = roomBoxes[i].min;
			//if (checkAABBvAABBCollision(bulletBox.getAABB(), &roomBox))
			{
				for (unsigned int j = 0; j < worldBoxes[i].size(); j++)
				{
					bool contin = false;

					//do or do not, there is a try again at half the position

					if (checkAABBvAABBCollision(&box, &worldBoxes[i][j].boundingBox))
					{
						//printf("%d %d \n", i, j); // test for abbs so they register

						//for each obb contained in that abb
						int size = worldBoxes[i][j].boundingBox.ObbBoxes.size();
						for (int n = 0; n < size; n++)
						{
							t = getSpherevOBBNorms(bPos, rad, &worldBoxes[i][j].boundingBox.ObbBoxes[n]);
							t.w = rad - t.w; //penetration depth instead of collision distance 
							if (t.w + FLT_EPSILON > 0 - FLT_EPSILON && t.w - FLT_EPSILON < rad + FLT_EPSILON)
							{
								t = vec4(normalize(vec3(t)), t.w);
								t.w = t.w * (4 - k);//gets the pendepth based on where in the dt we are
								return t;
							}
						}
					}
				}
			}
		}		
	}

	return vec4(-1, -1, -1, -1);
}

vec3 Physics::checkBulletVWorldCollision(vec3 bulletPos)
{
	vec3 collides = vec3(0, 0, 0);

	vec3 size = vec3(0.3f, 0.3f, 0.3f);
	bulletBox.setAABB(bulletPos, bulletPos + size, bulletPos - size);

	for (unsigned int i = 0; i < worldBoxes.size(); i++)
	{
		for (unsigned int j = 0; j < worldBoxes[i].size(); j++)
		{
			if (checkAABBvAABBCollision(&bulletBox.boundingBox, &worldBoxes[i][j].boundingBox))
			{
				collides = vec3(1, 1, 1);
				//spawnLactatingNipplesOfDoom();
			}
		}
	}
	return collides;
}

std::vector<vec4> Physics::checkPlayerVEffectCollision(glm::vec3 playerPos, unsigned int eType, int eid)
{
	std::vector<glm::vec4> collided;

	AABB box;
	float rad = playerBox.getSphere().radius;
	box.max = playerPos + vec3(rad, rad, rad);
	box.min = playerPos - vec3(rad, rad, rad);
	playerBox.setAABB(box);
	playerBox.setSphere(playerPos, rad);

	for (int i = 0; i < effectBoxes.size(); i++)
	{
		if (effectBoxes[i]->getEID() == eid)
		{
			if (effectBoxes[i]->getEType() == eType)
			{
				if (effectBoxes[i]->getEType() == 0)//Lightwall, aka OBB
				{
					collided = checkSpherevOBBlwCollision(playerBox, effectBoxes[i]->getCollisionMesh());
				}
				else if (effectBoxes[i]->getEType() == 1)//Lightwall, aka OBB
				{
					collided = checkSpherevSpheretdCollision(playerBox, effectBoxes[i]->getCollisionMesh());
				}
				else if (effectBoxes[i]->getEType() > 8)//False box, no collision
				{

				}
				else //evrything else is a sphere, if not, not my goddamn problem
				{
					collided = checkSpherevSphereCollision(playerBox, effectBoxes[i]->getCollisionMesh());
				}
			}
		}
	}

	for (int i = 0; i < collided.size(); i++)
	{
		if(eType == 0)
		if (collided[i].y > 0.01f || collided[i].y < -0.01f)
			collided[i].y = 0;
	}
	return collided;
}

bool Physics::checkPlayerVCaptureCollision(vec3 playerPos, int capID)
{
	for (int i = 0; i < captureBoxes.size(); i++)
	{
		if (captureBoxes[i].capID == capID)
		{
			AABB box;
			box.pos = playerPos;
			box.max = playerPos + playerBox.getSphere().radius;
			box.max = playerPos - playerBox.getSphere().radius;
			playerBox.setAABB(box);

			playerBox.setPos(playerPos);

			return checkPlayerVCapCollision(playerBox.getAABB(), captureBoxes[i]);
		}
	}
	return false;
}

std::vector<glm::vec4> Physics::checkBulletVEffectCollision(glm::vec3 bulletPos, vec3 bulletVel, vec3 bulletDir, unsigned int eType, int eid, float dt)
{
	std::vector<glm::vec4> cNorms;

	AABB box;
	vec3 bPos = bulletPos - (bulletVel * bulletDir * dt);
	float rad = bulletBox.getSphere().radius;
	box.max = bPos + vec3(rad, rad, rad);
	box.min = bPos - vec3(rad, rad, rad);
	bulletBox.setAABB(box);
	bulletBox.setSphere(bPos, rad);
	vec4 t;
	vec3 collisionNormal = vec3(0, 0, 0);

	float dtbyI = 0.0f;
	vec3 deltaDir = vec3(0);
	vec3 dirTimesVel = vec3(0);
	dtbyI = dt / 4;

	//bPos += bulletVel * bulletDir * (dt / (float)i);
	deltaDir = bulletDir * dtbyI;
	dirTimesVel = bulletVel * deltaDir;

	for (int k = 0; k < 4; k++)
	{

		if (cNorms.size() == 0)
		{
			bPos += dirTimesVel;
			box.max = bPos + vec3(rad, rad, rad);
			box.min = bPos - vec3(rad, rad, rad);
			bulletBox.setAABB(box);
			bulletBox.setSphere(bPos, rad);

			for (int i = 0; i < effectBoxes.size(); i++)
			{
				if (effectBoxes[i]->getEID() == eid)
				{
					if (effectBoxes[i]->getEType() == eType)
					{
						if (effectBoxes[i]->getEType() == 0)//Lightwall, aka OBB
						{
							cNorms = checkSpherevOBBlwCollision(bulletBox, effectBoxes[i]->getCollisionMesh());
						}
						else if (effectBoxes[i]->getEType() > 8)//False box, no collision
						{

						}
						else //evrything else is a sphere, if not, not my goddamn problem
						{
							cNorms = checkSpherevSphereCollision(bulletBox, effectBoxes[i]->getCollisionMesh());
						}
						if (cNorms.size() > 0)
						{
							for (int j = 0; j < cNorms.size(); j++)
							{
								cNorms[j].w *= (4 - k);
							}
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < cNorms.size(); i++)
	{
		if (cNorms[i].y > 0.01f || cNorms[i].y < -0.01f)//we only care bout them walls yo
			cNorms[i].y = 0;
	}

	return cNorms;
}

float Physics::addGravity(float dt)
{
	return GRAVITY * dt;
}

vec3 Physics::normalize(vec3 vec3)
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

bool Physics::removeEffect(int eid, int pid, unsigned int eType)
{
	int cpid = -1, ceid = -1;
	unsigned int ceType = 0;
	for (int i = 0; i < effectBoxes.size(); i++)
	{
		effectBoxes[i]->getIDs(ceType, cpid, ceid);

		if (ceid == eid && cpid == pid && ceType == eType)
		{
			delete effectBoxes[i];
			effectBoxes[i] = effectBoxes[effectBoxes.size() - 1];
			effectBoxes.pop_back();

		}
	}
	return true;
}

void Physics::receiveEffectBox(std::vector<float> eBox, unsigned int etype, int pID, int eID)
{
	//sphere
	//obb

	/*
	Center för sfärer, startpositionen för lightwalls
	0 = x
	1 = y
	2 = z

	3 = radie, eller höjd OM 4 FINNS

	slutposition för lightwalls
	4 = x2
	5 = y2
	6 = z2
	*/
	EffectMesh* effMesh = new EffectMesh;
	effMesh->init();
	CollideMesh temp;

	AABB aabb;
	Sphere sphere;
	if (eBox.size() > 4)
	{
		//OBB
		glm::vec3 pos1;
		glm::vec3 pos2;
		pos1.x = eBox[0];
		pos1.y = eBox[1];
		pos1.z = eBox[2];

		pos2.x = eBox[4];
		pos2.y = eBox[5];
		pos2.z = eBox[6];

		glm::vec3 pos, max, min;

		pos = (pos1 + pos2) / 2.0f;

		if (pos1.x > pos2.x)
		{
			max.x = pos1.x;
			min.x = pos2.x;
		}
		else
		{
			max.x = pos2.x;
			min.x = pos1.x;
		}

		if (pos1.z > pos2.z)
		{
			max.z = pos1.z;
			min.z = pos2.z;
		}
		else
		{
			max.z = pos2.z;
			min.z = pos1.z;
		}

		max.y = pos.y + eBox[3];
		min.y = pos.y - eBox[3];

		aabb.pos = pos;
		aabb.max = max;
		aabb.min = min;

		OBBloaded obbl;

		vec3 up = vec3(0, 1, 0);

		vec3 fwd = normalize(pos2 - pos1);

		vec3 n = normalize(cross(up, fwd));

		obbl.corners[0] = glm::vec4(pos1.x - 0.2f * n.x, pos.y - eBox[3], pos1.z + 0.2f * n.z, 1.0f);
		obbl.corners[1] = glm::vec4(pos1.x + 0.2f * n.x, pos.y - eBox[3], pos1.z + 0.2f * n.z, 1.0f);
		obbl.corners[2] = glm::vec4(pos1.x - 0.2f * n.x, pos.y + eBox[3], pos1.z + 0.2f * n.z, 1.0f);
		obbl.corners[3] = glm::vec4(pos1.x + 0.2f * n.x, pos.y + eBox[3], pos1.z + 0.2f * n.z, 1.0f);
		obbl.corners[4] = glm::vec4(pos2.x - 0.2f * n.x, pos.y + eBox[3], pos2.z - 0.2f * n.z, 1.0f);
		obbl.corners[5] = glm::vec4(pos2.x + 0.2f * n.x, pos.y + eBox[3], pos2.z - 0.2f * n.z, 1.0f);
		obbl.corners[6] = glm::vec4(pos2.x - 0.2f * n.x, pos.y - eBox[3], pos2.z - 0.2f * n.z, 1.0f);
		obbl.corners[7] = glm::vec4(pos2.x + 0.2f * n.x, pos.y - eBox[3], pos2.z - 0.2f * n.z, 1.0f);

		/*float angle;

		vec3 centeredPos1 = pos1 - pos;
		vec3 centeredPos2 = pos2 - pos;

		vec2 x = vec2(1, 0);
		vec2 normCentPos2 = glm::normalize(vec2(centeredPos2.x, centeredPos2.z));
		angle = dot(x, normCentPos2);
		angle = acos(angle);

		vec2 centPos1Angled = vec2(centeredPos1.x, centeredPos1.z);
		vec2 centPos2Angled = vec2(centeredPos2.x, centeredPos2.z);

		centPos1Angled.x = centeredPos1.x * cos(-angle) - centeredPos1.z * sin(-angle);
		centPos1Angled.y = centeredPos1.z * sin(-angle) + centeredPos1.x * cos(-angle);

		centPos2Angled.x = centeredPos2.x * cos(-angle) - centeredPos2.z * sin(-angle);
		centPos2Angled.y = centeredPos2.z * sin(-angle) + centeredPos2.x * cos(-angle);

		glm::vec3 corners[8];
		corners[0] = glm::vec3(centPos1Angled.x + 0.25f, -eBox[3], centPos1Angled.y - 0.25f);
		corners[1] = glm::vec3(centPos1Angled.x - 0.25f, -eBox[3], centPos1Angled.y - 0.25f);
		corners[2] = glm::vec3(centPos1Angled.x + 0.25f, eBox[3], centPos1Angled.y - 0.25f);
		corners[3] = glm::vec3(centPos1Angled.x - 0.25f, eBox[3], centPos1Angled.y - 0.25f);
		corners[4] = glm::vec3(centPos2Angled.x + 0.25f, eBox[3], centPos2Angled.y + 0.25f);
		corners[5] = glm::vec3(centPos2Angled.x - 0.25f, eBox[3], centPos2Angled.y + 0.25f);
		corners[6] = glm::vec3(centPos2Angled.x + 0.25f, -eBox[3], centPos2Angled.y + 0.25f);
		corners[7] = glm::vec3(centPos2Angled.x - 0.25f, -eBox[3], centPos2Angled.y + 0.25f);

		for (int i = 0; i < 8; i++)
		{
			glm::vec3 temp = corners[i];

			corners[i].x = temp.x * cos(PI - angle) + temp.z * sin(PI - angle);
			corners[i].z = -temp.z * sin(PI - angle) + temp.x * cos(PI - angle);
			corners[i] += pos;

			obbl.corners[i] = vec4(corners[i], 1);
		}*/



		OBB obb;
		obb.init(&obbl);
		obb.setPlanes();
		aabb.ObbBoxes.push_back(obb);

		temp.setAABB(aabb);

	}
	else
	{
		//SPHERE
		sphere.pos.x = eBox[0];
		sphere.pos.y = eBox[1];
		sphere.pos.z = eBox[2];
		sphere.radius = eBox[3];
		temp.setSphere(sphere);
	}
	effMesh->setCollisionMesh(temp);
	effMesh->setIDs(etype, pID, eID);
	effectBoxes.push_back(effMesh);

	/*
	Skapa 8 punkter från centpos1Angled & 2
	splitta punkterna med en tottal ofset på 0.5, dvs 0.25, i y/z-led (y i vec2, z i vec3)
	rotera tillbaks ALLA 8 punkter
	gör om de till vec3s igen
	lägg på originalpos
	mata in i obbl
	Lägg in sphere
	we done here

	BUT OPVER THERE THO
	lägg in SpherevSphere
	lägg in i Game att mata in effekter till physics
	i Physics::receiveEffectBox så ska du lägga in inmatning av lådorna
	där du har fler instruktioner för om det är obb eller sphere, dvs i den hör funktionen
	i checkPlayervEffect & checkBulletvEffect ska du skapa collisioner, spherevObb & spherevSphere
	skicka tillbaks collisionsträffar till game, med pid och eid och e-type (africa)
	*/

}

void Physics::receiveChunkBoxes(int chunkID, void* _cBoxes)
{
	std::vector<AABBloaded>* cBox = (std::vector<AABBloaded>*)_cBoxes;
	std::vector<AABBloaded> cBoxes = std::vector<AABBloaded>(*cBox);

	//chunkID isn't used, but we send it here anyway, JUST IN CASE

	//creates a vector, so we know information is copied
	std::vector<CollideMesh> cMeshes;
	CollideMesh temp;

	for (unsigned int n = 0; n < cBoxes.size(); n++)
	{
		temp = CollideMesh();
		//stores all the info in a tempmesh
		temp.setAABB(&cBoxes[n]);

		//adds it to our vector
		cMeshes.push_back(temp);
	}

	//Adds the vector to the WorldBox
	//This vector is all the collisionboxes in a chunk
	//so Chunk[0] will have it's collisionBoxes in worldBoxes[0]
	worldBoxes.push_back(cMeshes);
}

void Physics::receiveCap(int nrCaps, void* capBoxes)
{
	CaptureLoaded* caps = (CaptureLoaded*)capBoxes;

	for (int n = 0; n < nrCaps; n++)
	{
		captureBoxes.push_back(AABBCapPoint());
		captureBoxes[n].init(caps[n]);
	}

	delete [] caps;
}

void Physics::receivePlayerBox(std::vector<float> pBox, float rad)
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

	playerBox.setAABB(vec3(xPos, yPos, zPos), vec3(pBox[0], pBox[2], pBox[4]), vec3(pBox[1], pBox[3], pBox[5]));
	playerBox.setSphere(glm::vec3(xPos, yPos, zPos), rad);
}

void Physics::receivePlayerRad(float rad)
{
	playerBox.setSphere(playerBox.getSphere().pos, rad);
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

		temp.setAABB(vec3(xPos, yPos, zPos), vec3(xSize, ySize, zSize), vec3(-xSize, -ySize, -zSize));

		worldBoxes[0].push_back(temp);
	}
}

void Physics::receiveRoomBoxes(void* _roomboxes)
{
	AABBcapLoaded* inRooms = (AABBcapLoaded*)_roomboxes;

	//nr of chunks
	int size = worldBoxes.size();
	for (int n = 0; n < size; n++)
	{
		roomBoxes.push_back(AABBCapPointDivide());
		roomBoxes[n].pos = vec3(inRooms[n].pos);
		roomBoxes[n].max = vec3(inRooms[n].max);
		roomBoxes[n].min = vec3(inRooms[n].min);
	}
}

Physics* CreatePhysics()
{
	return new Physics();
}