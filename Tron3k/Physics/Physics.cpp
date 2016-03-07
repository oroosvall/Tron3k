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
	//TODO:
	//INIT EVERYTHING
	return 1;
}

void Physics::initBulletBox()
{
	//TEMPORARY
	/*bulletBox.init();
	vec3 size = vec3(0.2f, 0.2f, 0.2f);
	bulletBox.setAABB(vec3(0, 0, 0), size, -size);
	bulletBox.setSphere(vec3(0, 0, 0), 0.4f);
	*/
	//bulletBox.setSize(size);
}

bool Physics::release()
{
	delete this;
	return 1;
}

//--------AABB Collisions--------//

bool Physics::checkAABBvAABBCollision(AABBSingle mesh1, AABBSingle mesh2)
{
	mesh1.max += FLT_EPSILON;
	mesh1.min -= FLT_EPSILON;

	mesh2.max += FLT_EPSILON;
	mesh2.min -= FLT_EPSILON;

	if (mesh1.max.x > mesh2.min.x && mesh1.min.x < mesh2.max.x)//x
		if (mesh1.max.y > mesh2.min.y && mesh1.min.y < mesh2.max.y)//y
			if (mesh1.max.z > mesh2.min.z && mesh1.min.z < mesh2.max.z)//z
				return true;

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
vec3 Physics::checkOBBvOBBCollision(CollideMesh mesh1, CollideMesh mesh2)
{
	glm::vec3 collided = glm::vec3(0, 0, 0);
	return collided;
}

vec4 Physics::checkOBBvCylinderCollision(Cylinder pMesh, OBB objMesh)
{
	glm::vec4 collided = glm::vec4(0, 0, 0, 0);

	//CIRCLE V PLANEEEEEEEEEEEEEEEEEEEEEEEEE

	vec3 cNormal = vec3(0, 1, 0);
	vec3 pNorm = vec3(0);
	vec3 ang = vec3(0);

	vec3 cylLine = (pMesh.pos + (pMesh.height / 2)) - (pMesh.pos - (pMesh.height / 2));

	for (int i = 0; i < 6; i++)
	{
		//TODO: ADD THINGS
		pNorm = objMesh.planes[i].n;

		ang = cross(cNormal, pNorm);

		vec3 lvP = checkLinevPlaneCollision((pMesh.pos + (pMesh.height / 2)), (pMesh.pos - (pMesh.height / 2)), objMesh.planes[i].p[0], objMesh.planes[i].p[1], objMesh.planes[i].p[2], objMesh.planes[i].p[3], objMesh.planes[i].n);

		if (length(lvP) > 0.00001f)
			if (dot(lvP - objMesh.planes[i].p[0], objMesh.planes[i].n) < 0.01f)
			{
				if (dot(normalize(lvP), objMesh.planes[i].n) < 0.0f)
					//if (length(bPos - sPos) > (length((lvP/* - (bulletDir * rad)*/)-sPos)))
						//bPos = lvP;// -theOBB->planes[p].n * rad;// - bulletDir * rad;

					collided = vec4(normalize(lvP), length(lvP));
			}

		if (length(collided) > 0.001f)
		{
			//we have collided with a plane above/beneath us
			return collided;
		}
		else
		{
			//now we will check the sides... oh boy
			//we do this by checking from pos and count it as a sphere, I guess?
			vec4 norm = getSpherevOBBNorms(pMesh.pos, pMesh.radius, &objMesh);
			//vec3 dir = normalize(vec3(t));
			//norm.w = pMesh.radius - norm.w; //penetration depth instead of collision distance 
			if (norm.w + FLT_EPSILON >= 0 - FLT_EPSILON && norm.w - FLT_EPSILON <= pMesh.radius + FLT_EPSILON)
			{
				//t = vec4(dir, t.w + 0.2f);
				//bulletPos = bPos;
				return norm;
			}
		}
		/*
		if (length(ang) < 0.001f) //planes are parallell
		{
			if (pMesh.pos.y - (pMesh.height / 2) - FLT_EPSILON < objMesh.planes[i].p[0].y + FLT_EPSILON && pMesh.pos.y + (pMesh.height / 2) + FLT_EPSILON > objMesh.planes[i].p[0].y - FLT_EPSILON)
			{
				//a point of the plane is within the cylinder

			}
			else
			{
				//
			}
		}*/
	}

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
glm::vec4 Physics::checkSpherevSphereCollision(Sphere mesh1, Sphere mesh2)
{
	glm::vec4 collided = vec4(0, 0, 0, 0);

	glm::vec3 dist = mesh1.pos - mesh2.pos;
	float radius = mesh1.radius + mesh2.radius;

	if (length(dist) <= radius)
	{
		//collision
		return (vec4(dist, radius - length(dist)));

	}
	return collided;
}

glm::vec4 Physics::checkSpherevSpheretdCollision(Sphere mesh1, Sphere mesh2)
{
	glm::vec4 collided = vec4(0, 0, 0, 0);

	glm::vec3 dist = mesh1.pos - mesh2.pos;
	float radius = mesh1.radius + mesh2.radius;
	float minRad = mesh1.radius - 2.0f + mesh2.radius;

	if (length(dist) <= radius && length(dist) >= minRad)
	{
		//collision
		vec3 d = normalize(dist);

		//if(inside)sd
		if (abs(length(dist) - minRad) < abs(length(dist) - radius))
		{
			return (vec4(normalize(dist), -abs(minRad - length(dist)))); //works for when we're inside sphere
		}
		else
		{
			/*works if we outside sphere*/
			//dist = dist - d * mesh1.getSphere().radius;
			return (vec4(normalize(dist), abs(radius - length(dist))));
		}
	}
	return collided;
}

glm::vec4 Physics::checkSpherevOBBlwCollision(Sphere mesh1, OBB mesh2) //mesh 1 = Sphere, mesh2 = obb
{

	vec4 t = glm::vec4(0);

	mesh2.setPlanes();

	//printf("%d %d \n", i, j); // test for abbs so they register
	float rad = mesh1.radius;
	t = getSpherevOBBlwNorms(mesh1.pos, mesh1.radius, &mesh2);
	t.w = rad - t.w; //penetration depth instead of collision distance 
	if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= mesh1.radius + FLT_EPSILON)
	{

		vec3 t3 = vec3(t);
		if (length(t3) > 0)
			normalize(t3);
		t = vec4(t3, t.w);
	}
	else
		t = vec4(0);
	return t;
}
//--------------//--------------//

vec3 Physics::checkLinevPlaneCollision(vec3 l1, vec3 l2, vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 n)
{
	vec3 p = vec3(0, 0, 0);

	float x = 1;
	vec3 l = (l2 - l1);
	vec3 l3 = l;
	l3 = normalize(l);

	p = normalize(cross((p1 - p2), (p3 - p2)));

	float d = -((p2.x * p.x) + (p2.y * p.y) + (p2.z + p.z));

	//the equation for the plane is now p + d

	if (dot(l3, n) <= 0.0f + FLT_EPSILON && dot(l3, n) >= 0.0f - FLT_EPSILON)
	{
		x = (dot(p2 - l1, n) / FLT_EPSILON);
	}
	else
	{
		x = (dot(p2 - l1, n) / (dot(l3, n)));
	}

	if (length(l3 * x) - FLT_EPSILON <= length(l) + FLT_EPSILON && x + FLT_EPSILON >= 0 - FLT_EPSILON ||
		x + FLT_EPSILON >= 0 - FLT_EPSILON && x - FLT_EPSILON <= length(l) + FLT_EPSILON)
	{
		//TO ADD: LIMIT THE PLANE
		//THANKS BASED EMIL <3

		vec3 inter = l1 + (l3 * x);

		glm::vec3 v = normalize(inter - p1);
		glm::vec3 u = normalize(inter - p3);

		vec3 v1 = normalize(p2 - p1);
		vec3 v2 = normalize(p3 - p2);
		vec3 v3 = normalize(p4 - p3);
		vec3 v4 = normalize(p1 - p4);

		float test1 = dot(v1, v);
		float test2 = dot(v3, u);

		if (test1 + FLT_EPSILON >= 0.0001f - FLT_EPSILON && test2 + FLT_EPSILON >= 0.0001f - FLT_EPSILON)
		{
			// Y check
			v = normalize(inter - p2);
			u = normalize(inter - p4);
			test1 = dot(v2, v);
			test2 = dot(v4, u);

			if (test1 + FLT_EPSILON >= 0.0001f - FLT_EPSILON && test2 + FLT_EPSILON >= 0.0001f - FLT_EPSILON)
			{
				return inter;
			}
		}
	}

	return vec3(0);
}

//------Normal Calculators------//
std::vector<vec3> Physics::getCollisionNormal(AABB* aabb1, AABB* aabb2)
{
	std::vector<vec3> norms;
	/*vec3 l1 = vec3(aabb1->pos);
	vec3 l2 = vec3(aabb2->pos);

	CollideMesh temp;
	temp.init();



	temp.setAABB(vec3(aabb2->pos), vec3(aabb2->max), vec3(aabb2->min));

	//P2 should be center point, just in case
	vec3 p1, p2, p3;
	p1 = p2 = p3 = vec3(0, 0, 0);

	vec3 collides1 = vec3(-1);
	vec3 collides2 = vec3(-1);

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



	if ((collides1 != vec3(-1) || collides2 != vec3(-1)) && !collideZ)
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



	if ((collides1 != vec3(-1) || collides2 != vec3(-1)) && !collideZ)
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


	if ((collides1 != vec3(-1) || collides2 != vec3(-1)) && !collideY)
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


	if ((collides1 != vec3(-1) || collides2 != vec3(-1)) && !collideY)
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



	if ((collides1 != vec3(-1) || collides2 != vec3(-1)) && !collideX)
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



	if ((collides1 != vec3(-1) || collides2 != vec3(-1)) && !collideX)
	{
		//Left plane collision
		collideX = true;
		norms.push_back(vec3(-collides1, 0, 0));
	}
*/
	return norms;

}

vec4 Physics::getSpherevOBBNorms(vec3 pos, float rad, OBB* obb, vec3 backDir, bool isBullet)
{
	vec4 t = vec4(0);
	vec4 closest = vec4(0);
	closest.w = FLT_MAX;

	//test vs all planes
	for (int n = 0; n < 6; n++)
	{
		// send inverse plane normal as dir
		// t.w will be distance to the wall -1 if no wall was close enough
		t = obb->planes[n].intersects(pos, -obb->planes[n].n, rad);

		// if a valid intersection found
		//plane intersection will always be closer than all other intersections on the obb
		if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON * 10)
		{

			return t;
		}
	}

	//if no plane intersections found
	//test all lines  

	for (int n = 0; n < 12; n++)
	{
		t = obb->lines[n].sphere_intersects(pos, rad);

		if (!isBullet)
		{
			if (obb->lines[n].line.y < -FLT_EPSILON || obb->lines[n].line.y > FLT_EPSILON)
			{
				if (t.w > 0)
				{
					if (t.w < closest.w) // a new closest dist was found
					{
						closest = t;
					}
				}
			}
			else //This is for flat lines. Else we occasionally got stuck on slopes. Without this we would also sometimes fall through lines
			{
				if (t.w > 0)
					if (t.w < closest.w) // a new closest dist was found
					{
						closest.x = t.x;
						closest.y = abs(t.y);
						closest.z = t.z;
						closest.w = t.w;
					}
			}
		}
		if (t.w < FLT_MAX && isBullet)
		{
			vec3 direc = normalize(obb->lines[n].plane1Normal + obb->lines[n].plane2Normal);

			closest.x = direc.x;
			closest.y = direc.y;
			closest.z = direc.z;
		}
	}
	//if we found a line intersection it will always be closer
	//than all the corner intersections
	if (closest.w - FLT_EPSILON <= rad + FLT_EPSILON)
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

			if (isBullet)
			{
				closest.x = obb->cornerNorms[n].x;
				closest.y = obb->cornerNorms[n].y;
				closest.z = obb->cornerNorms[n].z;
			}
			else
			{
				closest.x = test.x;
				closest.y = test.y;
				closest.z = test.z;
			}
			closest.w = test_len;
		}
	}

	// if the closest one if further than sphere rad = no intersection
	if (closest.w + FLT_EPSILON > rad - FLT_EPSILON)
		closest.w = FLT_MAX;

	if (closest.w - FLT_EPSILON <= rad + FLT_EPSILON)
		return closest;
	//return vec4(FLT_MAX);

	bool outside = false;
	vec3 smallest = vec3(999999, 999999, 999999);
	int planeN = -1;
	float l = 0.0f;
	//float d = 99;

	vec3 pDir = pos - obb->pos;
	pDir = normalize(pDir);
	float dotVal = -2;

	for (int n = 0; n < 6 && !outside; n++)
	{
		//Are we inside the obb?
		//ood
		vec3 p = (obb->planes[n].p[0]);// +obb->planes[n].p[2]) * 0.5f;
		vec3 dir = p - pos;
		//d = dot(dir, obb->planes[n].n);

		if (dot(pDir, obb->planes[n].n) > dotVal)
		{
			dotVal = dot(pDir, obb->planes[n].n);
			planeN = n;
		}

		if (dot(normalize(dir), obb->planes[n].n) >= 0.0f - FLT_EPSILON) //pointing the same way as normal, point is "behind" the plane
		{
			//behind plane
			//do stuff
			if (length(smallest) > length(dir))
			{
				smallest = dir;
			//	planeN = n;
			}
			if (l < length(dir))
				l = length(dir);
		}
		else//if it's infront of any plane, we're not inside it
			outside = true;
	}
	if (planeN != -1)
	{
		smallest = obb->planes[planeN].n;
		vec3 p = obb->planes[planeN].p[0];
		l = length(p - pos);
	}
	/*
	int ThisIsMyCode = 1;
	while (ThisIsMyCode)
		printf("My Code Is Amazing!\n");
		*/
	if (!outside && backDir != vec3(0) && dotVal >= 0.0f)// && l < rad)
	{
		closest = vec4(smallest, l);
	}
	else
		return vec4(FLT_MAX);

	return closest;
}

vec4 Physics::getSpherevOBBlwNorms(vec3 pos, float rad, OBB* obb)
{
	vec4 t = vec4(0);
	vec4 closest = vec4(0);
	closest.w = FLT_MAX;

	//test vs all planes
	for (int n = 0; n < 6; n++)
	{
		// send inverse plane normal as dir
		// t.w will be distance to the wall -1 if no wall was close enough
		t = obb->planes[n].intersects(pos, -obb->planes[n].n, rad);

		// if a valid intersection found
		//plane intersection will always be closer than all other intersections on the obb

		//possible fix:
		//tiny edge lightwlls, a possible fix is to add line checks for the lines not parallell to the ground
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
//--------------//--------------//

vec3 Physics::checkPlayerVPlayerCollision(vec3 playerPos1, vec3 playerPos2)
{
	playerBox.setPos(playerPos1);
	playerBox.setPlayerSize();
	PlayerMesh p2;
	p2.setPos(playerPos2);
	p2.setPlayerSize(playerBox.getPlayerSize());
	//p2.setSize(playerBox.getSize());

	bool ret = checkAABBvAABBCollision(playerBox.getAABB(), p2.getAABB());
	if (ret)
		return vec3(1, 1, 1);
	return vec3(0, 0, 0);
}

vec3 Physics::checkPlayerVBulletCollision(vec3 playerPos, vec3 bulletPos, vec3 size, vec3 bulletDir, vec3 bulletVel, float dt, bool &collided, float bModifier)
{
	collided = false;
	playerBox.setPos(playerPos);
	playerBox.setPlayerSize(size);
	//TEMPORARY

	bulletBox.setPos(bulletPos);
	bulletBox.setSize(bModifier);
	AABBSingle box = bulletBox.getAABB();
	vec3 origPos = bulletPos - (bulletVel * bulletDir * dt);
	box.min = origPos - bulletBox.getWorldSize();

	vec3 collide = vec3(0, 0, 0);// checkAABBCollision(playerBox, bulletBox);

	if (checkAABBvAABBCollision(playerBox.getAABB(), box))
	{
		collided = true;
		return normalize(bulletPos - playerPos);
	}
	return collide;
}

std::vector<vec4> Physics::PlayerVWorldCollision(vec3 playerPos, vec3 playerDir, vec3 playerVel, float dt)
{
	playerBox.setPos(playerPos);
	playerBox.setWorldSize();
	vec3 origPos = playerPos - (playerVel * dt);
	AABBSingle box = playerBox.getAABB();
	float rad = playerBox.getSphere().radius;
	float abbrad = rad + 0.01f;

	box.min = origPos - playerBox.getWorldSize();
	box.max += abbrad;
	box.min -= abbrad;

	std::vector<vec4> cNorms;
	vec4 t = vec4(0);
	vec3 collisionNormal = vec3(0, 0, 0);

	/*while (1) {
		float youShouldProbablyNotAddASemiColonAfterThisLine = 0
	}*/

	//each abb
	for (unsigned int j = 0; j < roomBoxes[0].getRoomBoxes()->size(); j++)
	{
		if (checkAABBvAABBCollision(box, roomBoxes[0].getSpecificBox(j)->getAABB()))
		{
			int size = roomBoxes[0].getSpecificBox(j)->getOBBSize();

			for (int n = 0; n < size; n++)
			{
				if (checkAABBvAABBCollision(box, roomBoxes[0].getSpecificBox(j)->getOBB(n)->aabb))
				{
					t = vec4(0);

					t = getSpherevOBBNorms(playerPos, rad, roomBoxes[0].getSpecificBox(j)->getOBB(n), -normalize(playerVel));
					//t = checkOBBvCylinderCollision(playerBox.getCylinder(), *roomBoxes[0].getSpecificBox(j)->getOBB(n));
					t.w = rad - t.w; //penetration depth instead of collision distance 
					if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
					{
						t = vec4(normalize(vec3(t)), t.w);
						if (t.y > 0.98f)
							t.y = 1;
						else if (t.y < -0.98f)
							t.y = -1;
						//t = vec4(normalize(vec3(t)), t.w);
						cNorms.push_back(t);
					}
					else
					{
						t = getSpherevOBBNorms(origPos, rad, roomBoxes[0].getSpecificBox(j)->getOBB(n), -normalize(playerVel));
						//t = checkOBBvCylinderCollision(playerBox.getCylinder(), *roomBoxes[0].getSpecificBox(j)->getOBB(n));
						t.w = rad - t.w; //penetration depth instead of collision distance 
						if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
						{
							t = vec4(normalize(vec3(t)), t.w);
							if (t.y > 0.98f)
								t.y = 1;
							else if (t.y < -0.98f)
								t.y = -1;
							//t = vec4(normalize(vec3(t)), t.w);
							cNorms.push_back(t);
						}
						/*else
						{
							OBB* theOBB = roomBoxes[0].getSpecificBox(j)->getOBB(n);
							bool collidedWithPlane = false;
							vec3 pPos = playerPos;
							for (int p = 0; p < 6; p++)
							{
								vec3 lvP = checkLinevPlaneCollision(origPos, playerPos, theOBB->planes[p].p[0], theOBB->planes[p].p[1], theOBB->planes[p].p[2], theOBB->planes[p].n);

								if (length(lvP) > 0.00001f)
									if (dot(lvP - theOBB->planes[p].p[0], theOBB->planes[p].n) < 0.01f)
									{
										if (dot(normalize(lvP - origPos), theOBB->planes[p].n) < 0.0f)
											if (length(pPos - origPos) > (length((lvP/* - (bulletDir * rad))-origPos)))
							/*					pPos = lvP;// -theOBB->planes[p].n * rad;// - bulletDir * rad;
										collidedWithPlane = true;
									}
							}
							t = getSpherevOBBNorms(pPos, rad, roomBoxes[0].getSpecificBox(j)->getOBB(n), -normalize(playerVel));
							//t = checkOBBvCylinderCollision(playerBox.getCylinder(), *roomBoxes[0].getSpecificBox(j)->getOBB(n));
							t.w = rad - t.w; //penetration depth instead of collision distance
							if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
							{
								t = vec4(normalize(vec3(t)), t.w);
								if (t.y > 0.98f)
									t.y = 1;
								else if (t.y < -0.98f)
									t.y = -1;
								cNorms.push_back(t);
							}
						}*/
					}
				}
			}
		}
	}

	AABBSingle* roomBox;
	//each chunk
	bool passed = false;
	for (unsigned int i = 1; i < roomBoxes.size(); i++)
	{
		//culling player intersection tests vs room aabbs
		roomBox = roomBoxes[i - 1].getAABB();
		passed = false;
		if (box.max.x > roomBox->min.x && box.min.x < roomBox->max.x)//x
			if (box.max.y > roomBox->min.y && box.min.y < roomBox->max.y)//y
				if (box.max.z > roomBox->min.z && box.min.z < roomBox->max.z)//y
					passed = true;

		if (passed)
		{
			//each abb
			for (unsigned int j = 0; j < roomBoxes[i].getRoomBoxes()->size(); j++)
			{
				if (checkAABBvAABBCollision(box, roomBoxes[i].getSpecificBox(j)->getAABB()))
				{
					//for each obb contained in that abb
					int size = roomBoxes[i].getSpecificBox(j)->getOBBSize();
					for (int n = 0; n < size; n++)
					{
						if (checkAABBvAABBCollision(box, roomBoxes[i].getSpecificBox(j)->getOBB(n)->aabb))
						{
							t = getSpherevOBBNorms(playerPos, rad, roomBoxes[i].getSpecificBox(j)->getOBB(n), -normalize(playerVel));
							//t = checkOBBvCylinderCollision(playerBox.getCylinder(), *roomBoxes[i].getSpecificBox(j)->getOBB(n));
							t.w = rad - t.w; //penetration depth instead of collision distance 
							if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
							{
								t = vec4(normalize(vec3(t)), t.w);
								if (t.y > 0.98f)
									t.y = 1;
								else if (t.y < -0.98f)
									t.y = -1;
								//	t = vec4(normalize(vec3(t)), t.w);
								cNorms.push_back(t);
							}
							else
							{
								t = getSpherevOBBNorms(origPos, rad, roomBoxes[i].getSpecificBox(j)->getOBB(n), -normalize(playerVel));
								//	t = checkOBBvCylinderCollision(playerBox.getCylinder(), *roomBoxes[i].getSpecificBox(j)->getOBB(n));
								t.w = rad - t.w; //penetration depth instead of collision distance 
								if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
								{
									t = vec4(normalize(vec3(t)), t.w);
									if (t.y > 0.98f)
										t.y = 1;
									else if (t.y < -0.98f)
										t.y = -1;
									//	t = vec4(normalize(vec3(t)), t.w);
									cNorms.push_back(t);
								}
								/*else
								{
									OBB* theOBB = roomBoxes[i].getSpecificBox(j)->getOBB(n);
									bool collidedWithPlane = false;
									vec3 pPos = playerPos;
									for (int p = 0; p < 6; p++)
									{
										vec3 lvP = checkLinevPlaneCollision(origPos, playerPos, theOBB->planes[p].p[0], theOBB->planes[p].p[1], theOBB->planes[p].p[2], theOBB->planes[p].n);

										if (length(lvP) > 0.00001f)
											if (dot(lvP - theOBB->planes[p].p[0], theOBB->planes[p].n) < 0.01f)
											{
												if (dot(normalize(lvP - origPos), theOBB->planes[p].n) < 0.0f)
													if (length(pPos - origPos) > (length((lvP/* - (bulletDir * rad))-origPos)))
									/*					pPos = lvP;// -theOBB->planes[p].n * rad;// - bulletDir * rad;
												collidedWithPlane = true;
											}
									}
									t = getSpherevOBBNorms(pPos, rad, roomBoxes[i].getSpecificBox(j)->getOBB(n), -normalize(playerVel));
									//t = checkOBBvCylinderCollision(playerBox.getCylinder(), *roomBoxes[0].getSpecificBox(j)->getOBB(n));
									t.w = rad - t.w; //penetration depth instead of collision distance
									if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
									{
										t = vec4(normalize(vec3(t)), t.w);
										if (t.y > 0.98f)
											t.y = 1;
										else if (t.y < -0.98f)
											t.y = -1;
										cNorms.push_back(t);
									}
								}*/
							}
						}
					}
				}
			}
		}
	}
	return cNorms;
}

vec4 Physics::BulletVWorldCollision(vec3 &bulletPos, vec3 bulletVel, vec3 bulletDir, float dt)
{
	bulletBox.setPos(bulletPos);
	vec4 t = vec4(0);
	AABBSingle box = bulletBox.getAABB();
	float rad = bulletBox.getSphere().radius;

	vec3 origPos = bulletPos - (bulletVel * bulletDir * dt);
	vec3 bPos = bulletPos;
	vec3 ePos = bulletPos + bulletDir * rad;
	vec3 sPos = origPos - bulletDir * rad;
	std::vector<vec4> cNorms;
	std::vector<vec3> posAdjs;
	box.pos = (bulletPos);
	box.max = bulletPos + bulletBox.getWorldSize();
	box.min = origPos - bulletBox.getWorldSize(); //This should make sure that we never pass through walls???

	bool collidedWithPlane = false;
	for (unsigned int j = 0; j < roomBoxes[0].getRoomBoxes()->size(); j++)
	{
		//each chunk
		if (checkAABBvAABBCollision(box, roomBoxes[0].getSpecificBox(j)->getAABB()))
		{
			//for each obb contained in that abb
			int size = roomBoxes[0].getSpecificBox(j)->getOBBSize();
			for (int n = 0; n < size; n++)
			{
				OBB* theOBB = roomBoxes[0].getSpecificBox(j)->getOBB(n);

				if (checkAABBvAABBCollision(box, roomBoxes[0].getSpecificBox(j)->getOBB(n)->aabb))
				{
					collidedWithPlane = false;
					for (int p = 0; p < 6; p++)
					{
						vec3 lvP = checkLinevPlaneCollision(sPos, ePos, theOBB->planes[p].p[0], theOBB->planes[p].p[1], theOBB->planes[p].p[2], theOBB->planes[p].p[3], theOBB->planes[p].n);

						if (length(lvP) > 0.00001f)
							if (dot(lvP - theOBB->planes[p].p[0], theOBB->planes[p].n) < 0.01f)
							{
								if (dot(normalize(lvP - sPos), theOBB->planes[p].n) < 0.0f)
									if (length(bPos - sPos) > (length((lvP/* - (bulletDir * rad)*/)-sPos)))
										bPos = lvP;// -theOBB->planes[p].n * rad;// - bulletDir * rad;
								collidedWithPlane = true;
							}
					}

					if (collidedWithPlane)
					{
						//bPos -= bulletDir * rad;
						//WE HAVE COLLISION
						t = getSpherevOBBNorms(bPos, rad, theOBB, -bulletDir, true);
						vec3 dir = normalize(vec3(t));
						t.w = rad - t.w; //penetration depth instead of collision distance 
						if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
						{
							t = vec4(dir, t.w + 0.2f);
							bPos -= dir* rad;
							posAdjs.push_back(bPos);
							cNorms.push_back(t);
						}

						bPos = bPos - (rad * normalize(bulletDir) * 0.99f);
						t = getSpherevOBBNorms(bPos, rad, theOBB, -bulletDir, true);
						dir = normalize(vec3(t));
						t.w = rad - t.w;
						if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
						{
							t = vec4(dir, t.w + 0.2f);
							bPos -= dir* rad;
							cNorms.push_back(t);
							posAdjs.push_back(bPos);
						}

						t = getSpherevOBBNorms(bulletPos, rad, theOBB, -bulletDir, true);
						dir = normalize(vec3(t));
						t.w = rad - t.w; //penetration depth instead of collision distance 
						if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
						{
							t = vec4(dir, t.w + 0.2f);
							cNorms.push_back(t);
							posAdjs.push_back(bulletPos);
						}
						t = getSpherevOBBNorms(origPos, rad, theOBB, -bulletDir, true);
						dir = normalize(vec3(t));
						t.w = rad - t.w;
						if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
						{
							t = vec4(dir, t.w + 0.2f);
							cNorms.push_back(t);
							posAdjs.push_back(origPos);
						}
					}
				}
			}
		}
	}

	AABBSingle* roomBox;
	origPos = bulletPos - (bulletVel * bulletDir * dt);
	bPos = bulletPos;
	ePos = bulletPos + bulletDir * rad;
	sPos = origPos - bulletDir * rad;

	bool passed = false;
	for (unsigned int i = 1; i < roomBoxes.size(); i++)
	{
		//culling player intersection tests vs room aabbs
		if (i > 0)
		{
			roomBox = roomBoxes[i - 1].getAABB();
			passed = false;
			if (box.max.x > roomBox->min.x && box.min.x < roomBox->max.x)//x
				if (box.max.y > roomBox->min.y && box.min.y < roomBox->max.y)//y
					if (box.max.z > roomBox->min.z && box.min.z < roomBox->max.z)//y
						passed = true;

			if (passed)
			{
				//each chunk
				//each abb
				for (unsigned int j = 0; j < roomBoxes[i].getRoomBoxes()->size(); j++)
				{
					if (checkAABBvAABBCollision(box, roomBoxes[i].getSpecificBox(j)->getAABB()))
					{
						//for each obb contained in that abb
						int size = roomBoxes[i].getSpecificBox(j)->getOBBSize();
						for (int n = 0; n < size; n++)
						{
							OBB* theOBB = roomBoxes[i].getSpecificBox(j)->getOBB(n);
							if (checkAABBvAABBCollision(box, roomBoxes[i].getSpecificBox(j)->getOBB(n)->aabb))
							{
								collidedWithPlane = false;
								for (int p = 0; p < 6; p++)
								{
									vec3 lvP = checkLinevPlaneCollision(sPos, ePos, theOBB->planes[p].p[0], theOBB->planes[p].p[1], theOBB->planes[p].p[2], theOBB->planes[p].p[3], theOBB->planes[p].n);

									if (length(lvP) > 0.001f)
										if (dot(lvP - theOBB->planes[p].p[0], theOBB->planes[p].n) < 0.001f)
										{
											if (dot(normalize(lvP - sPos), theOBB->planes[p].n) < 0.0f)
												if (length(bPos - -sPos) > (length((lvP/* - (bulletDir * rad)*/)-sPos)))
													bPos = lvP;// -theOBB->planes[p].n * rad;// -bulletDir * rad;
											collidedWithPlane = true;
										}
								}

								if (collidedWithPlane)
								{
									//bPos -= bulletDir * rad;
									//WE HAVE COLLISION
									t = getSpherevOBBNorms(bPos, rad, theOBB, -bulletDir, true);
									vec3 dir = normalize(vec3(t));
									t.w = rad - t.w; //penetration depth instead of collision distance 
									if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
									{
										t = vec4(dir, t.w + 0.2f);
										bPos -= dir* rad;
										cNorms.push_back(t);
										posAdjs.push_back(bPos);
									}

									bPos = bPos - (rad * normalize(bulletDir) * 0.99f);
									t = getSpherevOBBNorms(bPos, rad, theOBB, -bulletDir, true);
									dir = normalize(vec3(t));
									t.w = rad - t.w;
									if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
									{
										t = vec4(dir, t.w + 0.2f);
										bPos -= dir* rad;
										posAdjs.push_back(bPos);
										cNorms.push_back(t);
									}

									t = getSpherevOBBNorms(bulletPos, rad, theOBB, -bulletDir, true);
									dir = normalize(vec3(t));
									t.w = rad - t.w; //penetration depth instead of collision distance 
									if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
									{
										t = vec4(dir, t.w + 0.2f);
										cNorms.push_back(t);
										posAdjs.push_back(bulletPos);
									}

									t = getSpherevOBBNorms(origPos, rad, theOBB, -bulletDir, true);
									dir = normalize(vec3(t));
									t.w = rad - t.w;
									if (t.w + FLT_EPSILON >= 0 - FLT_EPSILON && t.w - FLT_EPSILON <= rad + FLT_EPSILON)
									{
										t = vec4(dir, t.w + 0.2f);
										cNorms.push_back(t);
										posAdjs.push_back(origPos);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (cNorms.size() > 0)
	{
		vec4 finalNorm = vec4(0);
		for (int i = 0; i < cNorms.size(); i++)
		{
			finalNorm += cNorms[i];
		}
		finalNorm = vec4(normalize(vec3(finalNorm)), finalNorm.w / cNorms.size());
		return finalNorm;
	}
	if (posAdjs.size() > 0)
	{
		vec3 fPos = vec3(0);
		for (int i = 0; i < posAdjs.size(); i++)
		{
			fPos += posAdjs[i];
		}
		fPos /= posAdjs.size();
		bulletPos = fPos;
	}

	return vec4(-1, -1, -1, -1);
}

vec4 Physics::checkPlayerVEffectCollision(glm::vec3 playerPos, unsigned int eType, int eid, bool &col)
{
	glm::vec4 collided = vec4(0);
	col = false;
	playerBox.setPos(playerPos);
	playerBox.setWorldSize();
	AABBSingle box = playerBox.getAABB();
	Sphere sphere = playerBox.getSphere();


	//THIS ALL NEEDS TO CHANGE
	//EFFECTMESH NEEDS TO CHANGE
	int effectSize = (int)effectBoxes.size();
	for (int i = 0; i < effectSize; i++)
	{
		if (effectBoxes[i]->getEID() == eid && effectBoxes[i]->getEType() == eType)
		{
			if (checkAABBvAABBCollision(box, effectBoxes[i]->getAABB()))
			{
				if (effectBoxes[i]->getEType() == eType)
				{
					if (eType == 0)//Lightwall, aka OBB
					{
						collided = checkSpherevOBBlwCollision(sphere, *effectBoxes[i]->getOBB());
						if (collided != vec4(0, 0, 0, 0))
							col = true;
					}
					else if (eType == 1)//ThunderDome aka sphere
					{
						collided = checkSpherevSpheretdCollision(sphere, effectBoxes[i]->getSphere());
						if (collided != vec4(0, 0, 0, 0))
							col = true;
					}
					else if (eType > 10 || eType < 0)//False box, no collision
					{
						collided = vec4(0);
						col = false;
					}
					else //evrything else is a sphere, if not, not my goddamn problem
					{
						collided = checkSpherevSphereCollision(sphere, effectBoxes[i]->getSphere());
						if (collided != vec4(0, 0, 0, 0))
							col = true;
					}
				}
			}
		}
	}


	if (eType == 0)
		collided.y = 0;

	return collided;
}

bool Physics::checkPlayerVCaptureCollision(vec3 playerPos, int capID)
{
	playerBox.setPos(playerPos);
	playerBox.setWorldSize();
	AABBSingle box = playerBox.getAABB();
	int captureBoxesSize = (int)captureBoxes.size();
	bool collided = false;
	for (int i = 0; i < captureBoxesSize && !collided; i++)
	{
		if (captureBoxes[i].getCapID() == capID)
		{
			if (checkAABBvAABBCollision(playerBox.getAABB(), captureBoxes[i].getAABB()))
			{
				int size = captureBoxes[i].getSubAABBs().size();

				for (int j = 0; j < size && !collided; j++)
				{
					collided = checkAABBvAABBCollision(playerBox.getAABB(), captureBoxes[i].getSubAABBs()[j]);
				}
			}
		}
	}
	return collided;
}

vec4 Physics::checkBulletVEffectCollision(glm::vec3 &bulletPos, vec3 bulletVel, vec3 bulletDir, unsigned int eType, int eid, float dt)
{
	glm::vec4 collided = vec4(0);

	AABBSingle box = bulletBox.getAABB();

	vec3 origPos = bulletPos - (bulletVel * bulletDir * dt);
	vec3 bPos = bulletPos;
	bulletBox.setPos(bulletPos);
	box.pos = (bulletPos);
	box.max = bulletPos + bulletBox.getWorldSize();
	box.min = origPos - bulletBox.getWorldSize(); //This should make sure that we never pass through walls???


	Sphere sphere = bulletBox.getSphere();
	float rad = sphere.radius;
	vec4 t;
	vec3 collisionNormal = vec3(0, 0, 0);

	vec3 ePos = bulletPos + bulletDir * rad;
	vec3 sPos = origPos - bulletDir * rad;



	int effectBoxesSize = (int)effectBoxes.size();
	for (int i = 0; i < effectBoxesSize; i++)
	{
		if (effectBoxes[i]->getEID() == eid && effectBoxes[i]->getEType() == eType)
		{
			if (checkAABBvAABBCollision(box, effectBoxes[i]->getAABB()))
			{
				if (effectBoxes[i]->getEType() == eType)
				{
					if (eType == 0)//Lightwall, aka OBB
					{

						//collided = checkSpherevOBBlwCollision(sphere, *effectBoxes[i]->getOBB());
						OBB* theOBB = effectBoxes[i]->getOBB();

						bool collidedWithPlane = false;
						for (int p = 0; p < 6; p++)
						{
							vec3 lvP = checkLinevPlaneCollision(sPos, ePos, theOBB->planes[p].p[0], theOBB->planes[p].p[1], theOBB->planes[p].p[2], theOBB->planes[p].p[3], theOBB->planes[p].n);

							if (length(lvP) > 0.00001f)
								if (dot(lvP - theOBB->planes[p].p[0], theOBB->planes[p].n) < 0.01f)
								{
									if (dot(normalize(lvP - sPos), theOBB->planes[p].n) < 0.0f)
										if (length((bPos/* - (bulletDir * rad)*/)-sPos) > (length(lvP - sPos)))
											bPos = lvP - theOBB->planes[p].n * rad;// -bulletDir * rad;
									collidedWithPlane = true;
								}
						}
						if (collidedWithPlane)
						{
							sphere.pos = bPos;

							collided = checkSpherevOBBlwCollision(sphere, *theOBB);
							vec3 dir = normalize(vec3(collided));

							if (collided.w + FLT_EPSILON >= 0 - FLT_EPSILON && collided.w - FLT_EPSILON <= rad + FLT_EPSILON)
							{
								if (dot(dir, normalize(bPos - sPos)) > -0.00001f)
									dir *= -1;
								collided = vec4(dir, collided.w + 0.2f);
								bulletPos = bPos;
							}
						}



					}
					else if (eType == 1)//ThunderDome aka sphere
					{
						collided = checkSpherevSpheretdCollision(sphere, effectBoxes[i]->getSphere());
					}
					else if (eType > 9)//False box, no collision
					{

					}
					else //evrything else is a sphere, if not, not my goddamn problem
					{
						collided = checkSpherevSphereCollision(sphere, effectBoxes[i]->getSphere());
					}

					if (eType == 0)
						if (collided.y > 0.01f || collided.y < -0.01f)
							collided.y = 0;
				}
			}
		}
	}
	return collided;
}

bool Physics::checkEffectVEffectCollision(unsigned int eType1, unsigned int eType2, int eid1, int eid2, int pid1, int pid2)
{
	bool collided = false;

	EffectMesh* eMesh = nullptr;
	int effectBoxesSize = (int)effectBoxes.size();
	for (int i = 0; i < effectBoxesSize; i++)
	{
		if (effectBoxes[i]->getEType() == eType1 && effectBoxes[i]->getEID() == eid1 && effectBoxes[i]->getPID() == pid1)
			eMesh = effectBoxes[i];
	}

	if (eMesh != nullptr)
	{
		Sphere sphere = eMesh->getSphere();

		int effectBoxesSize = (int)effectBoxes.size();
		for (int i = 0; i < effectBoxesSize; i++)
		{
			if (effectBoxes[i]->getEID() == eid2 && effectBoxes[i]->getEType() == eType2 && effectBoxes[i]->getPID() == pid2)
			{
				if (checkAABBvAABBCollision(eMesh->getAABB(), effectBoxes[i]->getAABB()))
				{
					if (effectBoxes[i]->getEType() == eType2)
					{
						if (eType2 == 0)//Lightwall, aka OBB
						{
							vec4 col = checkSpherevOBBlwCollision(sphere, *effectBoxes[i]->getOBB());
							//collided = checkSpherevOBBlwCollision(sphere, *effectBoxes[i]->getOBB());
							/*OBB* theOBB = effectBoxes[i]->getOBB();

							bool collidedWithPlane = false;
							for (int p = 0; p < 6; p++)
							{
								vec3 lvP = checkLinevPlaneCollision(sPos, ePos, theOBB->planes[p].p[0], theOBB->planes[p].p[1], theOBB->planes[p].p[2], theOBB->planes[p].n);

								if (length(lvP) > 0.00001f)
									if (dot(lvP - theOBB->planes[p].p[0], theOBB->planes[p].n) < 0.01f)
									{
										if (dot(normalize(lvP - sPos), theOBB->planes[p].n) < 0.0f)
											if (length((bPos/* - (bulletDir * rad))-sPos) > (length(lvP - sPos)))
												bPos = lvP - theOBB->planes[p].n * rad;// -bulletDir * rad;
										collidedWithPlane = true;
									}
							}
							if (collidedWithPlane)
							{
								sphere.pos = bPos;

								collided = checkSpherevOBBlwCollision(sphere, *theOBB);
								vec3 dir = normalize(vec3(collided));

								if (collided.w + FLT_EPSILON >= 0 - FLT_EPSILON && collided.w - FLT_EPSILON <= rad + FLT_EPSILON)
								{
									if (dot(dir, normalize(bPos - sPos)) > -0.00001f)
										dir *= -1;
									collided = vec4(dir, collided.w + 0.2f);
									bulletPos = bPos;
								}
							}

							*/
							if (col != vec4(0, 0, 0, 0))
								collided = true;

						}
						else if (eType2 == 1)//ThunderDome aka sphere
						{
							vec4 col = checkSpherevSphereCollision(sphere, effectBoxes[i]->getSphere());
							if (col != vec4(0, 0, 0, 0))
								collided = true;
						}
						else if (eType2 > 9)//False box, no collision
						{
							collided = false;
						}
						else //evrything else is a sphere, if not, not my goddamn problem
						{
							vec4 col = checkSpherevSphereCollision(sphere, effectBoxes[i]->getSphere());
							if (col != vec4(0, 0, 0, 0))
								collided = true;
						}
					}
				}
			}
		}
	}
	return collided;
}

float Physics::addGravity(float dt)
{
	return GRAVITY * dt;
}

vec3 Physics::normalize(vec3 vec3)
{
	float len = abs(vec3.x) + abs(vec3.y) + abs(vec3.z);

	if (len > 0)
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
	int effectBoxesSize = effectBoxes.size();
	for (int i = 0; i < effectBoxesSize; i++)
	{
		effectBoxes[i]->getIDs(ceType, cpid, ceid);

		if (ceid == eid && cpid == pid && ceType == eType)
		{
			delete effectBoxes[i];
			effectBoxes[i] = effectBoxes[effectBoxes.size() - 1];
			effectBoxes.pop_back();
			effectBoxesSize--;

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

	AABBSingle aabb;
	OBB obb;
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

		obbl.corners[0] = glm::vec4(pos1.x - 0.15f * n.x, pos.y - eBox[3], pos1.z + 0.15f * n.z, 1.0f);
		obbl.corners[1] = glm::vec4(pos1.x + 0.15f * n.x, pos.y - eBox[3], pos1.z + 0.15f * n.z, 1.0f);
		obbl.corners[2] = glm::vec4(pos1.x - 0.15f * n.x, pos.y + eBox[3], pos1.z + 0.15f * n.z, 1.0f);
		obbl.corners[3] = glm::vec4(pos1.x + 0.15f * n.x, pos.y + eBox[3], pos1.z + 0.15f * n.z, 1.0f);
		obbl.corners[4] = glm::vec4(pos2.x - 0.15f * n.x, pos.y + eBox[3], pos2.z - 0.15f * n.z, 1.0f);
		obbl.corners[5] = glm::vec4(pos2.x + 0.15f * n.x, pos.y + eBox[3], pos2.z - 0.15f * n.z, 1.0f);
		obbl.corners[6] = glm::vec4(pos2.x - 0.15f * n.x, pos.y - eBox[3], pos2.z - 0.15f * n.z, 1.0f);
		obbl.corners[7] = glm::vec4(pos2.x + 0.15f * n.x, pos.y - eBox[3], pos2.z - 0.15f * n.z, 1.0f);

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


		obb.init(&obbl);
		obb.setPlanes();
		effMesh->setAABB(aabb);
		effMesh->setOBB(obb);
		effMesh->setPos(aabb.pos);

	}
	else
	{
		//SPHERE
		sphere.pos.x = eBox[0];
		sphere.pos.y = eBox[1];
		sphere.pos.z = eBox[2];
		sphere.radius = eBox[3];

		aabb.pos = sphere.pos;
		aabb.max = aabb.pos + vec3(eBox[3], eBox[3], eBox[3]);
		aabb.min = aabb.pos - vec3(eBox[3], eBox[3], eBox[3]);
		effMesh->setAABB(aabb);
		effMesh->setSphere(sphere);
		effMesh->setPos(sphere.pos);
	}
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

	WorldMesh wMesh;
	AABBSingle aabb;
	OBB obb;
	roomBoxes.push_back(RoomMesh());
	for (unsigned int n = 0; n < cBoxes.size(); n++)
	{
		temp = CollideMesh();
		//stores all the info in a tempmesh
		temp.setAABB(&cBoxes[n]);

		//adds it to our vector
		cMeshes.push_back(temp);
		wMesh.setBoxes(cBoxes[n]);

		roomBoxes[chunkID].addBox(wMesh);
	}
	roomBoxes[chunkID].setRoomID(chunkID);

	//Adds the vector to the WorldBox
	//This vector is all the collisionboxes in a chunk
	//so Chunk[0] will have it's collisionBoxes in worldBoxes[0]

	//worldBoxes.push_back(cMeshes);
}

void Physics::receiveCap(int nrCaps, void* capBoxes)
{
	CaptureLoaded* caps = (CaptureLoaded*)capBoxes;

	AABBSingle aabb;
	CaptureMesh cMesh;
	for (int n = 0; n < nrCaps; n++)
	{
		cMesh.setBoxes(caps[n]);

		captureBoxes.push_back(cMesh);
		captureBoxes[n].setCapID(n);
	}

	delete[] caps;
}

void Physics::receivePlayerBox(std::vector<float> pBox, float rad)
{
	float xPos, yPos, zPos;
	float xSize, ySize, zSize;

	xSize = (abs(pBox[0]) + abs(pBox[1])) / 2;
	ySize = (abs(pBox[2]) + abs(pBox[3])) / 2;
	zSize = (abs(pBox[4]) + abs(pBox[5])) / 2;

	xPos = (pBox[0] + pBox[1]) / 2;
	yPos = (pBox[2] + pBox[3]) / 2;
	zPos = (pBox[4] + pBox[5]) / 2;

	//playerBox.setAABB(vec3(xPos, yPos, zPos), vec3(pBox[0], pBox[2], pBox[4]), vec3(pBox[1], pBox[3], pBox[5]));
	//playerBox.setSphere(glm::vec3(xPos, yPos, zPos), rad);
}

void Physics::receivePlayerRad(float rad)
{
	Sphere sphere;
	sphere.pos = playerBox.getPos();
	sphere.radius = rad;
	playerBox.setSphere(sphere);

	playerBox.setWorldSize(vec3(rad + 0.1f, rad + 0.1f, rad + 0.1f));
}

void Physics::receiveRoomBoxes(void* _roomboxes)
{
	AABBcapLoaded* inRooms = (AABBcapLoaded*)_roomboxes;

	//nr of chunks
	int size = roomBoxes.size();
	AABBSingle aabb;


	for (int n = 0; n < size; n++)
	{
		aabb.pos = vec3(inRooms[n].pos);
		aabb.max = vec3(inRooms[n].max);
		aabb.min = vec3(inRooms[n].min);

		roomBoxes[n].setAABB(aabb);
	}
}

void Physics::cullingPointvsRoom(glm::vec3* pos, int* arr_interIDs, int& interCount, int maxsize)
{
	interCount = 0;

	int roomBoxesSize = (int)roomBoxes.size();
	for (int i = 0; i < roomBoxesSize; i++)
	{
		if (pos->x > roomBoxes[i].getAABB()->min.x && pos->x < roomBoxes[i].getAABB()->max.x)//x
			if (pos->y > roomBoxes[i].getAABB()->min.y && pos->y < roomBoxes[i].getAABB()->max.y)//y
				if (pos->z > roomBoxes[i].getAABB()->min.z && pos->z < roomBoxes[i].getAABB()->max.z)//y
				{
					arr_interIDs[interCount] = i + 1;
					interCount++;
					if (interCount == maxsize)
						return;
				}
	}
}

Physics* CreatePhysics()
{
	return new Physics();
}