#include "Special.h"

int Special::getSpecialId()
{
	int ret = specialId;
	if (specialId != -1)
	{
		specialId++;
		specialId % 255;
	}
	return ret;
}