#include "AnimationData.h"

#include <fstream>

using std::ios;

void AnimData::load(std::string fileName)
{
	header.jointCount = 0;
	header.keyCount = 0;

	std::ifstream file;
	file.open(fileName, ios::in | ios::binary);

	if (file.is_open())
	{
		file.read((char*)&header, sizeof(AnimDataHeader));

		keyFrames = new KeyFrame[header.keyCount];
		
		for (int i = 0; i < header.keyCount; i++)
		{
			keyFrames[i].jointTransform = new glm::mat4[header.jointCount];
			file.read((char*)&keyFrames[i].jointTransform[0], sizeof(glm::mat4)*header.jointCount);
		}

		file.close();
	}
}

void AnimData::release()
{
	for (int i = 0; i < header.keyCount; i++)
	{
		delete[] keyFrames[i].jointTransform;
	}
	delete[] keyFrames;
}
