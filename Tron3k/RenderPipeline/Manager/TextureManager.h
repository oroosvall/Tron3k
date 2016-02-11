#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "../Streaming/TextureStreamer.h"
#include <thread>
#include <vector>

enum TEXTURE_STATE
{
	TEXTURE_LOADED,
	TEXTURE_STREAMING,
	TEXTURE_UNLOADED,
};

enum TEXTURE_FALLBACK
{
	DIFFUSE_FB,
	NORMAL_FB,
	NORMAL_FULL_FB,
	GLOW_FB
};

struct TextureInfo
{
	GLuint textureID;
	std::string texturePath;

	GLuint lastTextureSlot;

	TEXTURE_STATE state;

	float timeNotUsed;

};

class TextureManager
{
private:

	std::vector<TextureInfo> textureList;

	std::thread textureStreamingThread;

	GLuint* textureSlotBinds;
	int maxTextureUnitSize;
	int textureUnitCounter;

	GLuint defaultDiffuse;
	GLuint defaultNormal;
	GLuint defaultNormal_full;
	GLuint defaultGlow;

	GLuint defaultDiffuseSlot;
	GLuint defaultNormalSlot;
	GLuint defaultNormal_fullSlot;
	GLuint defaultGlowSlot;

public:

	void init();
	void release();

	void update(float dt);

	unsigned int createTexture(std::string path);

	void bindTexture(unsigned int &textureID, GLuint shader, GLuint shaderLocation, TEXTURE_FALLBACK fallback);
	void bindDefault(GLuint shader, GLuint shaderLocation, TEXTURE_FALLBACK fallback);

	void bind(TextureInfo &ti, GLuint shader, GLuint shaderLocation);

	static TextureManager* gTm;
};


#endif