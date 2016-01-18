#pragma once
#include "../Special.h"
#include <GLFW\glfw3.h>

class WallJump :
	public Special
{

private:
public:
	WallJump();
	~WallJump();

	void init();
	int update(float deltaTime);

	bool allowedToActivate(Player* p);
};

