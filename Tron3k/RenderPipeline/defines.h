#ifndef DEFINES_H
#define	DEFINES_H

struct Vertex3
{
	float x, y, z;
};

struct Vertex4
{
	float x, y, z, w;
};

struct Vertex
{
	float x, y, z;
	float n1, n2, n3;
	float u, v;
};

struct Vertex11
{
	float x, y, z;
	float u, v;
	float nx, ny, nz;
	float tx, ty, tz;
};

struct AnimVertex
{
	float x, y, z;
	float u, v;
	float nx, ny, nz;
	float tx, ty, tz;
	int b1, b2, b3, b4;
	float w1, w2, w3, w4;
};

struct Keyframe
{
	glm::mat4* jointTransform;
};

struct Animation
{
	Keyframe* keyFrames;
};

struct Index
{
	unsigned int i1, i2, i3;
};

#endif