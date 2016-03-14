#include "TextureManager.h"

#include "../Texture.h"

TextureManager* TextureManager::gTm = 0;

void TextureManager::init()
{
	textureStreamingThread = std::thread(&streamingThread);

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnitSize);

	textureSlotBinds = new GLuint[maxTextureUnitSize];

	textureUnitCounter = 0;

	defaultDiffuse =	loadTexture("GameFiles/Textures/Blank_d.dds");
	defaultNormal =		loadTexture("GameFiles/Textures/Blank_n.DDS");
	defaultNormal_full = loadTexture("GameFiles/Textures/Full_glow_normalmap.png");
	defaultGlow =		loadTexture("GameFiles/Textures/Blank_g.DDS");

	defaultDiffuseSlot = GL_TEXTURE0;
	defaultNormalSlot = GL_TEXTURE0;
	defaultNormal_fullSlot = GL_TEXTURE0;
	defaultGlowSlot = GL_TEXTURE0;

	gTm = this;
}

void TextureManager::release()
{

	glDeleteTextures(1, &defaultDiffuse);
	glDeleteTextures(1, &defaultNormal);
	glDeleteTextures(1, &defaultGlow);

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i].state == TEXTURE_LOADED)
		{
			glDeleteTextures(1, &textureList[i].textureID);
		}
	}

	delete[] textureSlotBinds;

	stremingThreadRunning = false;

	textureStreamingThread.join();

	typedef std::map<GLuint*, StreamedData>::iterator it_type;

	for (auto iterator = streamingQueue.begin(); iterator != streamingQueue.end(); iterator++)
	{
		streamingQueue.erase(iterator);
	}

	for (it_type iterator = dataQueue.begin(); iterator != dataQueue.end(); iterator++) {
		
		delete[] (char*)iterator->second.data;
	}

	streamingQueue.clear();
	dataQueue.clear();

	dataQueueEmpty = dataQueue.empty();

}

void TextureManager::update(float dt)
{
	unsigned int pos = 0;
	GLuint texture = uploadStreamedData(pos);
	if (texture && pos < textureList.size())
	{
		textureList[pos].textureID = texture;
		textureList[pos].state = TEXTURE_LOADED;
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i].state == TEXTURE_LOADED)
		{
			textureList[i].timeNotUsed += dt;
			if (textureList[i].timeNotUsed > 30.0f)
			{
				printf("Texture %d have not been used for 30 seconds unloading\n", i);
				glDeleteTextures(1, &textureList[i].textureID);
				textureList[i].textureID = 0;
				textureList[i].state = TEXTURE_UNLOADED;
				textureList[i].timeNotUsed = 0.0f;
			}
		}
		else if (textureList[i].state == TEXTURE_STREAMING)
		{
			textureList[i].timeNotUsed += dt;
			if (textureList[i].timeNotUsed > 5.0f)
			{
				printf("Texture %d have not been loaded in 5 seconds marking as unloaded\n", i);
				glDeleteTextures(1, &textureList[i].textureID);
				textureList[i].textureID = 0;
				textureList[i].state = TEXTURE_UNLOADED;
				textureList[i].timeNotUsed = 0.0f;
			}
		}
		textureList[i].timeNotUsed = 0.0f;
	}


}

unsigned int TextureManager::createTexture(std::string path)
{

	TextureInfo ti;

	ti.lastTextureSlot = GL_TEXTURE0;
	ti.state = TEXTURE_UNLOADED;
	ti.textureID = 0;
	ti.timeNotUsed = 0.0f;

	ti.texturePath = path;

	int retVal = textureList.size();

	bool found = false;

	ti.streamingID = retVal;

	//ti.textureID = loadTexture(path);

	for (size_t i = 0; i < textureList.size() && !found; i++)
	{
		if (textureList[i].texturePath == path)
		{
			retVal = i;
			found = true;
		}
	}
	if(!found)
		textureList.push_back(ti);

	return retVal;

}

bool TextureManager::PNGSize(const char* fileName, unsigned int &x, unsigned int &y)
{
	std::ifstream file(fileName, std::ios_base::binary | std::ios_base::in);

	if (!file.is_open() || !file)
	{
		file.close();
		return false;
	}

	file.seekg(8, std::ios_base::cur);
	file.seekg(4, std::ios_base::cur);
	file.seekg(4, std::ios_base::cur);

	__int32 width, height;
	
	file.read((char*)&width, 4);
	file.read((char*)&height, 4);


	#define SWAPWORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
	#define SWAPLONG(x) MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))
	//x = ntohl(width);
	//y = ntohl(height);

	x = SWAPLONG(width);
	y = SWAPLONG(height);

	file.close();

	return true;
}


//void TextureManager::bindTexture(unsigned int &textureID, GLuint shader, GLuint shaderLocation, TEXTURE_FALLBACK fallback, bool invokeUpdate)
//{
//	if (invokeUpdate)
//		update(0);
//	if (!textureList.empty() && textureID < textureList.size())
//	{
//		TextureInfo* ti = &textureList[textureID];
//	
//		if (ti->state == TEXTURE_UNLOADED)
//		{
//			if (addToStreamQueue(&textureID, ti->texturePath))
//			{
//				ti->state = TEXTURE_STREAMING;
//			}
//			bindDefault(shader, shaderLocation, fallback);
//		}
//		else if (ti->state == TEXTURE_STREAMING)
//		{
//			bindDefault(shader, shaderLocation, fallback);
//		}
//		else if (ti->state == TEXTURE_LOADED)
//		{
//			bind(*ti, shader, shaderLocation);
//			ti->timeNotUsed = 0.0f;
//		}
//	
//	}
//	else
//	{
//		bindDefault(shader, shaderLocation, fallback);
//	}
//
//}

void TextureManager::bindTextureOnly(unsigned int &textureID, TEXTURE_FALLBACK fallback, bool invokeUpdate)
{
	if (invokeUpdate)
		update(0);
	if (!textureList.empty() && textureID < textureList.size())
	{
		TextureInfo* ti = &textureList[textureID];

		if (ti->state == TEXTURE_UNLOADED)
		{
			if (addToStreamQueue(&ti->streamingID, ti->texturePath))
			{
				ti->state = TEXTURE_STREAMING;
			}

			bindDefaultOnly(fallback);

		}
		else if (ti->state == TEXTURE_STREAMING)
		{
			//bindDefault(shader, shaderLocation, fallback);
			bindDefaultOnly(fallback);
		}
		else if (ti->state == TEXTURE_LOADED)
		{
			glBindTexture(GL_TEXTURE_2D, ti->textureID);
			ti->timeNotUsed = 0.0f;
		}

	}
	else
	{

		bindDefaultOnly(fallback);

	}
}

//void TextureManager::bindDefault(GLuint shader, GLuint textureLocation, TEXTURE_FALLBACK fallback)
//{
//
//	GLuint texture = 0;
//	GLuint* slot = 0;
//
//	if (fallback == DIFFUSE_FB)
//	{
//		texture = defaultDiffuse;
//		slot = &defaultDiffuseSlot;
//	}
//	else if (fallback == NORMAL_FB)
//	{
//		texture = defaultNormal;
//		slot = &defaultNormalSlot;
//	}
//	else if (fallback == NORMAL_FULL_FB)
//	{
//		texture = defaultNormal_full;
//		slot = &defaultNormal_fullSlot;
//	}
//	else if (fallback == GLOW_FB)
//	{
//		texture = defaultGlow;
//		slot = &defaultGlowSlot;
//	}
//
//	//f (textureSlotBinds[*slot - GL_TEXTURE0] != texture)
//	//
//		*slot = GL_TEXTURE0 + textureUnitCounter;
//		textureUnitCounter = ((textureUnitCounter + 1) % (maxTextureUnitSize-1));
//
//	//	textureSlotBinds[*slot - GL_TEXTURE0] = texture;
//	//}
//	
//	glActiveTexture(*slot);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	
//	glProgramUniform1i(shader, textureLocation, *slot - GL_TEXTURE0);
//
//}

void TextureManager::bindDefaultOnly(TEXTURE_FALLBACK fallback)
{

	GLuint texture = 0;

	if (fallback == DIFFUSE_FB)
	{
		texture = defaultDiffuse;
	}
	else if (fallback == NORMAL_FB)
	{
		texture = defaultNormal;
	}
	else if (fallback == NORMAL_FULL_FB)
	{
		texture = defaultNormal_full;
	}
	else if (fallback == GLOW_FB)
	{
		texture = defaultGlow;
	}
	glBindTexture(GL_TEXTURE_2D, texture);
}

//void TextureManager::bind(TextureInfo &ti, GLuint shader, GLuint textureLocation)
//{
//	GLuint texture = ti.textureID;
//	GLuint slot = 0;// ti.lastTextureSlot;
//
//	//if (textureSlotBinds[slot - GL_TEXTURE0] != texture)
//	//{
//		slot = GL_TEXTURE0 + textureUnitCounter;
//		textureUnitCounter = ((textureUnitCounter + 1) % (maxTextureUnitSize-1));
//
//	//	textureSlotBinds[slot - GL_TEXTURE0] = texture;
//	//	ti.lastTextureSlot = slot;
//	//}
//
//	glActiveTexture(slot);
//	glBindTexture(GL_TEXTURE_2D, texture);
//
//	glProgramUniform1i(shader, textureLocation, slot - GL_TEXTURE0);
//
//}