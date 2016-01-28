#pragma once
#ifndef COLLIDEMESH_H
#define COLLIDEMESH_H

#include <vector>
#include"../../Dependencies/glm/glm/glm.hpp"

using namespace glm;

//Not sure if this should be a class, but it is temporarily

//corner defines
#define DLF 0
#define DRF 1
#define ULF 2
#define URF 3
#define DLB 4
#define DRB 5
#define ULB 6
#define URB 7

//These are read in engine, sent though core
struct OBBloaded
{
	vec4 corners[8];
};

struct AABBloaded
{
	vec4 pos;
	vec4 max;
	vec4 min;

	std::vector<OBBloaded> ObbBoxes;
};

struct AABBCapPointDivide
{
	vec4 pos;
	vec4 max;
	vec4 min;
};

struct AABBCapPoint
{
	vec4 pos;
	vec4 max;
	vec4 min;

	int capID;

	std::vector<AABBCapPointDivide> aabbs;
};

struct OBB_LINES
{
	vec3 point1;
	vec3 point2;
	vec3 line;
	vec3 line_inv;

	void init(vec3 _point1, vec3 _point2)
	{
		point1 = _point1;
		point2 = _point2;
		line = _point2 - _point1;
		line_inv = -line;
	}

	vec4 sphere_intersects(vec3 pos, float rad)
	{
		//line from corner to the sphere center
		vec3 p = pos - point2;

		// if sphere is behind the line, that means a corner will be closer than the closest line 
		if (dot(p, line_inv) < 0)
			return vec4(0, 0, 0, -1);

		p = pos - point1;

		if (dot(p, line) < 0)
			return vec4(0, 0, 0, -1);

		//project p on the line and multiply by the line
		float projlen = (dot(p, line) / dot(line, line));
		vec3 intersection = projlen* line;

		//normal
		intersection = p - intersection;
		float len = length(intersection);

		return vec4(normalize(intersection), len);
	}
};

struct PLANE
{
	vec3 p[4];
	vec3 n;

	//calculated once, used for intesection test
	vec3 v1;
	vec3 v2;
	vec3 v3;
	vec3 v4;

	void init(vec3 topleft, vec3 topright, vec3 botright, vec3 botleft)
	{
		p[0] = topleft;
		p[1] = topright;
		p[2] = botright;
		p[3] = botleft;

		v1 = normalize(p[1] - p[0]);
		v2 = normalize(p[2] - p[1]);
		v3 = normalize(p[3] - p[2]);
		v4 = normalize(p[0] - p[3]);

		n = normalize(cross(v1, v4));
	}

	//return collision normal, w holds intersection depth
	vec4 intersects(vec3 origin, vec3 dir, float len)
	{
		float denom = dot(n, dir);

		float d2 = length(denom);
		if (d2 > FLT_EPSILON)
		{
			vec3 d = p[0]-origin;
			vec3 dn = normalize(d);
			float t = dot(dn, n);
			t /= denom;

			if (t > 0) //if we traveled away from the portal
			{
				t = dot(d, n);
				t /= denom;
				if (len+FLT_EPSILON >= t-FLT_EPSILON) //if we traveled far enough to cross the plane
				{
					//check if the intersection is within the portal
					vec3 inter = origin + t * dir;

					// XZ CHECK

					vec3 v5 = normalize(inter - p[0]);
					vec3 v6 = normalize(inter - p[2]);

					float test1 = dot(v1, v5);
					float test2 = dot(v3, v6);

					if (test1 > FLT_EPSILON && test2 > FLT_EPSILON)
					{
						// Y check
						v5 = normalize(inter - p[1]);
						v6 = normalize(inter - p[3]);
						test1 = dot(v2, v5);
						test2 = dot(v4, v6);

						if (test1 > FLT_EPSILON && test2 > FLT_EPSILON)
						{
							//printf("%f \n", t);

							return vec4(n, t);
						}
					}
				}
			}
		}
		return vec4(0, 0, 0, -1);
	}
};

//----------------------//

struct OBB
{
	vec3 corners[8];

	PLANE planes[6];

	OBB_LINES lines[12];

	void init(OBBloaded* in)
	{
		for (int n = 0; n < 8; n++)
		{
			//convert obbloaded's corners from vec4 to obb's vec3 corners
			corners[n].x = in->corners[n].x; corners[n].y = in->corners[n].y; corners[n].z = in->corners[n].z;
		}

		//init planes
		planes[0].init(corners[2], corners[3], corners[1], corners[0]);
		planes[1].init(corners[4], corners[2], corners[0], corners[6]);
		planes[2].init(corners[5], corners[4], corners[6], corners[7]);
		planes[3].init(corners[3], corners[5], corners[7], corners[1]);
		planes[4].init(corners[4], corners[5], corners[3], corners[2]);
		planes[5].init(corners[7], corners[6], corners[0], corners[1]);

		//init all lines
		lines[0].init(corners[2], corners[3]);
		lines[1].init(corners[3], corners[1]);
		lines[2].init(corners[1], corners[0]);
		lines[3].init(corners[0], corners[2]);

		lines[4].init(corners[2], corners[4]);
		lines[5].init(corners[3], corners[5]);
		lines[6].init(corners[1], corners[7]);
		lines[7].init(corners[0], corners[6]);

		lines[8].init(corners[4], corners[5]);
		lines[9].init(corners[5], corners[7]);
		lines[10].init(corners[7], corners[6]);
		lines[11].init(corners[6], corners[4]);

	}

	void setPlanes()
	{
		//init planes
		planes[0].init(corners[2], corners[3], corners[1], corners[0]);
		planes[1].init(corners[4], corners[2], corners[0], corners[6]);
		planes[2].init(corners[5], corners[4], corners[6], corners[7]);
		planes[3].init(corners[3], corners[5], corners[7], corners[1]);
		planes[4].init(corners[4], corners[5], corners[3], corners[2]);
		planes[5].init(corners[7], corners[6], corners[0], corners[1]);

		//init all lines
		lines[0].init(corners[2], corners[3]);
		lines[1].init(corners[3], corners[1]);
		lines[2].init(corners[1], corners[0]);
		lines[3].init(corners[0], corners[2]);

		lines[4].init(corners[2], corners[4]);
		lines[5].init(corners[3], corners[5]);
		lines[6].init(corners[1], corners[7]);
		lines[7].init(corners[0], corners[6]);

		lines[8].init(corners[4], corners[5]);
		lines[9].init(corners[5], corners[7]);
		lines[10].init(corners[7], corners[6]);
		lines[11].init(corners[6], corners[4]);
	}
};

struct AABB
{
	vec3 pos;
	vec3 max;
	vec3 min;

	std::vector<OBB> ObbBoxes;

	void init(AABBloaded* in)
	{
		pos.x = in->pos.x; pos.y = in->pos.y; pos.z = in->pos.z;
		max.x = in->max.x; max.y = in->max.y; max.z = in->max.z;
		min.x = in->min.x; min.y = in->min.y; min.z = in->min.z;

		int size = in->ObbBoxes.size();

		for (int n = 0; n < size; n++)
		{
			ObbBoxes.push_back(OBB());
			ObbBoxes[n].init(&in->ObbBoxes[n]);
		}
	}
};

struct Cylinder
{
	vec3 pos;

	float radius;
	float height;
};

struct AngledCylinder
{
	vec3 pos;
	vec3 normal;

	float radius;
	float height;
};

struct Sphere
{
	vec3 pos;

	float radius;
};

//contains several levels of meshes, one that is just a cube, and then one or several below that is a simple mesh, not sure how to solve yet, might work.
class CollideMesh
{
private:

	Cylinder cylinder;
	Sphere sphere;
	AngledCylinder angledCylinder;
	void getCylinderFromAABB();
	void getSphereFromAABB();
	void getAngCylinderFromAABB();
public:
	AABB boundingBox;
	AABBCapPoint capBox; //only used for capture boxes;
	CollideMesh();
	~CollideMesh();

	void init();

	void setAABB(vec3 pos, vec3 max, vec3 min);
	void setAABB(AABB aabb);
	void setAABB(AABBloaded* aabb);
	AABB* getAABB();



	void setCylinder(vec3 pos, float radius, float height);
	Cylinder getCylinder();

	void setSphere(vec3 pos, float radius);
	void setSphere(Sphere sphere);
	Sphere getSphere();

	void setPos(vec3 pos);
};
#endif