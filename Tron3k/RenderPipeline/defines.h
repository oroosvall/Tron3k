#ifndef DEFINES_H
#define	DEFINES_H

struct Vertex3
{
	float x, y, z;
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

struct Index
{
	unsigned int i1, i2, i3;
};

#endif