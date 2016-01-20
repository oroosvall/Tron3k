#include "WallJump.h"
#include "../../../Player.h"

WallJump::WallJump()
{
}


WallJump::~WallJump()
{
}

bool WallJump::allowedToActivate(Player* p)
{
	if (!p->getGrounded())
	{
		int size = 0;
		glm::vec4* cNorms = p->getCollisionNormalsForFrame(size);

		for (int c = 0; c < size; c++)
		{
			if (cNorms[c].y < 0.2f && cNorms[c].y > -0.2f)
			{
				printf("Walljump\n");
				return true;
			}
		}
	}
	return false;
}

void WallJump::init()
{
	this->activationCost = 15;
	specialType = SPECIAL_TYPE::WALLJUMP;
	key = GLFW_KEY_SPACE;
}

int WallJump::update(float deltaTime)
{
	return 0;
}