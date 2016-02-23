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
			if (cNorms[c].y < 0.5f && cNorms[c].y > -0.2f)
			{
				float angle = dot(vec3(cNorms[c]), p->getDir());
				if (angle < -0.8f || angle > 0.8f)
				{
					p->movmentSpecialAnimUse(0); // regular jump anim
					return true;
				}

				if(glm::cross(vec3(cNorms[c]), p->getDir()).y > 0)
					p->movmentSpecialAnimUse(2); // right anim
				else
					p->movmentSpecialAnimUse(1); // left anim

				return true;
			}
		}
	}
	return false;
}

void WallJump::init()
{
	this->activationCost = 20;
	specialType = SPECIAL_TYPE::WALLJUMP;
	isOnJumpKey = true;
}

int WallJump::update(float deltaTime)
{
	return 0;
}