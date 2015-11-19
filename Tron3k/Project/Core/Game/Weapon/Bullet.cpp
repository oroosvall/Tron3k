#include"Bullet.h"

Bullet::~Bullet()
{}

void Bullet::updateWorldMat()
{
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y;
	worldMat[2].w = pos.z;
}