#ifndef MESHTYPES_H
#define MESHTYPES_H

#include<vector>
#include"../../Dependencies/glm/glm/glm.hpp"

using namespace glm;

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
	glm::vec4 corners[8];
};

struct AABBloaded
{
	glm::vec4 pos;
	glm::vec4 max;
	glm::vec4 min;

	std::vector<OBBloaded> ObbBoxes;
};

struct AABBcapLoaded
{
	glm::vec4 pos;
	glm::vec4 max;
	glm::vec4 min;
};

struct CaptureLoaded
{
	int roomID;
	AABBcapLoaded bigAABB;
	int subcount;
	AABBcapLoaded* subabbs;
};

struct AABBSingle
{
	glm::vec3 pos;
	glm::vec3 max;
	glm::vec3 min;
};

struct AABBCapPoint
{
	glm::vec3 pos;
	glm::vec3 max;
	glm::vec3 min;

	int capID;

	std::vector<AABBSingle> aabbs;

	void init(CaptureLoaded in)
	{
		capID = in.roomID;
		pos = glm::vec3(in.bigAABB.pos);
		max = glm::vec3(in.bigAABB.max);
		min = glm::vec3(in.bigAABB.min);
		int size = in.subcount;

		for (int n = 0; n < size; n++)
		{
			aabbs.push_back(AABBSingle());
			aabbs[n].pos = glm::vec3(in.subabbs[n].pos);
			aabbs[n].max = glm::vec3(in.subabbs[n].max);
			aabbs[n].min = glm::vec3(in.subabbs[n].min);
		}
	}

};

struct OBB_LINES
{
	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 line;
	glm::vec3 line_inv;
	glm::vec3 plane1Normal;
	glm::vec3 plane2Normal;

	void init(glm::vec3 _point1, glm::vec3 _point2, glm::vec3 normal1, glm::vec3 normal2)
	{
		point1 = _point1;
		point2 = _point2;
		line = _point2 - _point1;
		line_inv = -line;
		plane1Normal = normal1;
		plane2Normal = normal2;
	}

	glm::vec4 sphere_intersects(glm::vec3 pos, float rad)
	{
		//line from corner to the sphere center
		glm::vec3 p = pos - point2;

		// if sphere is behind the line, that means a corner will be closer than the closest line 
		if (dot(p, line_inv) < 0)
			return glm::vec4(0, 0, 0, -1);

		p = pos - point1;

		if (dot(p, line) < 0)
			return glm::vec4(0, 0, 0, -1);

		//project p on the line and multiply by the line
		float projlen = (dot(p, line) / dot(line, line));
		glm::vec3 intersection = projlen* line;

		//normal
		intersection = p - intersection;
		float len = length(intersection);

		return glm::vec4(normalize(intersection), len);
	}
};

struct PLANE
{
	glm::vec3 p[4];
	glm::vec3 n;

	//calculated once, used for intesection test
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
	glm::vec3 v4;

	void init(glm::vec3 topleft, glm::vec3 topright, glm::vec3 botright, glm::vec3 botleft)
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
	glm::vec4 intersects(glm::vec3 origin, glm::vec3 dir, float len)
	{
		float denom = dot(n, dir);

		float d2 = glm::length(denom);
		float t = 0;
		if (d2 > 0)
		{
			glm::vec3 d = p[0] - origin;
			glm::vec3 dn = normalize(d);
			t = dot(dn, n);
			t /= denom;

			if (t + FLT_EPSILON >= 0 - FLT_EPSILON) //if we traveled away from the portal
			{
				t = dot(d, n);
				t /= denom;
				if (len + FLT_EPSILON >= t - FLT_EPSILON) //if we traveled far enough to cross the plane
				{
					//check if the intersection is within the portal
					glm::vec3 inter = origin + t * dir;

					// XZ CHECK

					glm::vec3 v5 = normalize(inter - p[0]);
					glm::vec3 v6 = normalize(inter - p[2]);

					float test1 = dot(v1, v5);
					float test2 = dot(v3, v6);

					if (test1 + FLT_EPSILON >= 0.0001f - FLT_EPSILON && test2 + FLT_EPSILON >= 0.0001f - FLT_EPSILON)
					{
						// Y check
						v5 = normalize(inter - p[1]);
						v6 = normalize(inter - p[3]);
						test1 = dot(v2, v5);
						test2 = dot(v4, v6);

						if (test1 + FLT_EPSILON >= 0.0001f - FLT_EPSILON && test2 + FLT_EPSILON >= 0.0001f - FLT_EPSILON)
						{
							//printf("%f \n", t);
							if (t < 0)
								t = 0;
							return glm::vec4(n, t);
						}
					}
				}
			}
		}
		return glm::vec4(0, 0, 0, -1);
	}
};

//----------------------//

struct OBB
{
	AABBSingle aabb; //aabb that encapsulates the obb.
	glm::vec3 pos;

	glm::vec3 corners[8];
	glm::vec3 cornerNorms[8];
	PLANE planes[6];

	OBB_LINES lines[12];

	void init(OBBloaded* in)
	{
		float maxX, maxY, maxZ, minX, minY, minZ;
		maxX = maxY = maxZ = -FLT_MAX;
		minX = minY = minZ = FLT_MAX;
		for (int n = 0; n < 8; n++)
		{
			//convert obbloaded's corners from glm::vec4 to obb's glm::vec3 corners
			corners[n].x = in->corners[n].x; corners[n].y = in->corners[n].y; corners[n].z = in->corners[n].z;

			if (corners[n].x > maxX)
				maxX = corners[n].x;
			if (corners[n].x < minX)
				minX = corners[n].x;
			if (corners[n].y > maxY)
				maxY = corners[n].y;
			if (corners[n].y < minY)
				minY = corners[n].y;
			if (corners[n].z > maxZ)
				maxZ = corners[n].z;
			if (corners[n].z < minZ)
				minZ = corners[n].z;

		}
		aabb.max.x = maxX; aabb.max.y = maxY; aabb.max.z = maxZ;
		aabb.min.x = minX; aabb.min.y = minY; aabb.min.z = minZ;
		aabb.pos.x = (maxX + minX) / 2;
		aabb.pos.y = (maxY + minY) / 2;
		aabb.pos.z = (maxZ + minZ) / 2;
		pos = aabb.pos;

		//init planes
		planes[0].init(corners[2], corners[3], corners[1], corners[0]);
		planes[1].init(corners[4], corners[2], corners[0], corners[6]);
		planes[2].init(corners[5], corners[4], corners[6], corners[7]);
		planes[3].init(corners[3], corners[5], corners[7], corners[1]);
		planes[4].init(corners[4], corners[5], corners[3], corners[2]);
		planes[5].init(corners[7], corners[6], corners[0], corners[1]);

		//init all lines
		lines[0].init(corners[2], corners[3], planes[0].n, planes[4].n);
		lines[1].init(corners[3], corners[1], planes[0].n, planes[3].n);
		lines[2].init(corners[1], corners[0], planes[0].n, planes[5].n);
		lines[3].init(corners[0], corners[2], planes[0].n, planes[1].n);

		lines[4].init(corners[2], corners[4], planes[1].n, planes[4].n);
		lines[5].init(corners[3], corners[5], planes[4].n, planes[3].n);
		lines[6].init(corners[1], corners[7], planes[3].n, planes[5].n);
		lines[7].init(corners[0], corners[6], planes[5].n, planes[1].n);

		lines[8].init(corners[4], corners[5], planes[2].n, planes[4].n);
		lines[9].init(corners[5], corners[7], planes[3].n, planes[2].n);
		lines[10].init(corners[7], corners[6], planes[5].n, planes[2].n);
		lines[11].init(corners[6], corners[4], planes[1].n, planes[2].n);

		cornerNorms[0] = normalize(planes[0].n + planes[1].n + planes[5].n);
		cornerNorms[1] = normalize(planes[0].n + planes[3].n + planes[5].n);
		cornerNorms[2] = normalize(planes[0].n + planes[1].n + planes[4].n);
		cornerNorms[3] = normalize(planes[0].n + planes[3].n + planes[4].n);
		cornerNorms[4] = normalize(planes[1].n + planes[2].n + planes[4].n);
		cornerNorms[5] = normalize(planes[2].n + planes[3].n + planes[4].n);
		cornerNorms[6] = normalize(planes[1].n + planes[2].n + planes[5].n);
		cornerNorms[7] = normalize(planes[2].n + planes[3].n + planes[5].n);
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
		lines[0].init(corners[2], corners[3], planes[0].n, planes[4].n);
		lines[1].init(corners[3], corners[1], planes[0].n, planes[3].n);
		lines[2].init(corners[1], corners[0], planes[0].n, planes[5].n);
		lines[3].init(corners[0], corners[2], planes[0].n, planes[1].n);

		lines[4].init(corners[2], corners[4], planes[1].n, planes[4].n);
		lines[5].init(corners[3], corners[5], planes[4].n, planes[3].n);
		lines[6].init(corners[1], corners[7], planes[3].n, planes[5].n);
		lines[7].init(corners[0], corners[6], planes[5].n, planes[1].n);

		lines[8].init(corners[4], corners[5], planes[2].n, planes[4].n);
		lines[9].init(corners[5], corners[7], planes[3].n, planes[2].n);
		lines[10].init(corners[7], corners[6], planes[5].n, planes[2].n);
		lines[11].init(corners[6], corners[4], planes[1].n, planes[2].n);

		cornerNorms[0] = normalize(planes[0].n + planes[1].n + planes[5].n);
		cornerNorms[1] = normalize(planes[0].n + planes[3].n + planes[5].n);
		cornerNorms[2] = normalize(planes[0].n + planes[1].n + planes[4].n);
		cornerNorms[3] = normalize(planes[0].n + planes[3].n + planes[4].n);
		cornerNorms[4] = normalize(planes[1].n + planes[2].n + planes[4].n);
		cornerNorms[5] = normalize(planes[2].n + planes[3].n + planes[4].n);
		cornerNorms[6] = normalize(planes[1].n + planes[2].n + planes[5].n);
		cornerNorms[7] = normalize(planes[2].n + planes[3].n + planes[5].n);
	}
};

struct AABB
{
	glm::vec3 pos;
	glm::vec3 max;
	glm::vec3 min;

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
	glm::vec3 pos;

	float radius;
	float height;
};

struct AngledCylinder
{
	glm::vec3 pos;
	glm::vec3 normal;

	float radius;
	float height;
};

struct Sphere
{
	glm::vec3 pos;

	float radius;
};
#endif