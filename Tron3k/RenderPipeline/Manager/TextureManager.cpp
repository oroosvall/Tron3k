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

}

void TextureManager::update(float dt)
{
	unsigned int pos = 0;
	GLuint texture = uploadStreamedData(pos);
	if (texture)
	{
		textureList[pos].textureID = texture;
		textureList[pos].state = TEXTURE_LOADED;
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i].state == TEXTURE_LOADED)
		{
			textureList[i].timeNotUsed += dt;
			if (textureList[i].timeNotUsed > 100.0f)
			{
				printf("Texture %d have not been used for 100seconds unloading\n", i);
				glDeleteTextures(1, &textureList[i].textureID);
				textureList[i].textureID = 0;
				textureList[i].state = TEXTURE_UNLOADED;
				textureList[i].timeNotUsed = 0.0f;
			}
		}
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

	textureList.push_back(ti);

	return retVal;

}

void TextureManager::bindTexture(unsigned int &textureID, GLuint shader, GLuint shaderLocation, TEXTURE_FALLBACK fallback)
{
	if (!textureList.empty() && textureID < textureList.size())
	{
		TextureInfo* ti = &textureList[textureID];
	
		if (ti->state == TEXTURE_UNLOADED)
		{
			if (addToStreamQueue(&textureID, ti->texturePath))
			{
				ti->state = TEXTURE_STREAMING;
			}
			bindDefault(shader, shaderLocation, fallback);
		}
		else if (ti->state == TEXTURE_STREAMING)
		{
			bindDefault(shader, shaderLocation, fallback);
		}
		else if (ti->state == TEXTURE_LOADED)
		{
			bind(*ti, shader, shaderLocation);
			ti->timeNotUsed = 0.0f;
		}
	
	}
	else
	{
		bindDefault(shader, shaderLocation, fallback);
	}

}

void TextureManager::bindDefault(GLuint shader, GLuint textureLocation, TEXTURE_FALLBACK fallback)
{

	GLuint texture = 0;
	GLuint* slot = 0;

	if (fallback == DIFFUSE_FB)
	{
		texture = defaultDiffuse;
		slot = &defaultDiffuseSlot;
	}
	else if (fallback == NORMAL_FB)
	{
		texture = defaultNormal;
		slot = &defaultNormalSlot;
	}
	else if (fallback == NORMAL_FULL_FB)
	{
		texture = defaultNormal_full;
		slot = &defaultNormal_fullSlot;
	}
	else if (fallback == GLOW_FB)
	{
		texture = defaultGlow;
		slot = &defaultGlowSlot;
	}

	//f (textureSlotBinds[*slot - GL_TEXTURE0] != texture)
	//
		*slot = GL_TEXTURE0 + textureUnitCounter;
		textureUnitCounter = ((textureUnitCounter + 1) % maxTextureUnitSize);

	//	textureSlotBinds[*slot - GL_TEXTURE0] = texture;
	//}
	
	glActiveTexture(*slot);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glProgramUniform1i(shader, textureLocation, *slot - GL_TEXTURE0);

}

void TextureManager::bind(TextureInfo &ti, GLuint shader, GLuint textureLocation)
{
	GLuint texture = ti.textureID;
	GLuint slot = ti.lastTextureSlot;

	//if (textureSlotBinds[slot - GL_TEXTURE0] != texture)
	//{
		slot = GL_TEXTURE0 + textureUnitCounter;
		textureUnitCounter = ((textureUnitCounter + 1) % maxTextureUnitSize);

	//	textureSlotBinds[slot - GL_TEXTURE0] = texture;
	//	ti.lastTextureSlot = slot;
	//}

	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, texture);

	glProgramUniform1i(shader, textureLocation, slot - GL_TEXTURE0);

}