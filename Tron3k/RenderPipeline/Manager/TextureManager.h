#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "../Streaming/TextureStreamer.h"
#include <thread>
#include <vector>
#include <fstream>
#include <winsock.h>


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
	unsigned int streamingID;
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

	GLuint defaultDiffuse;
	GLuint defaultNormal;
	GLuint defaultNormal_full;
	GLuint defaultGlow;

	GLuint defaultDiffuseSlot;
	GLuint defaultNormalSlot;
	GLuint defaultNormal_fullSlot;
	GLuint defaultGlowSlot;

public:

	int textureUnitCounter;

	void init();
	void release();

	void update(float dt);

	unsigned int createTexture(std::string path);
	bool PNGSize(const char* fileName, unsigned int &x, unsigned int &y);
	//void bindTexture(unsigned int &textureID, GLuint shader, GLuint shaderLocation, TEXTURE_FALLBACK fallback, bool invokeUpdate = false);
	void bindTextureOnly(unsigned int &textureID, TEXTURE_FALLBACK fallback, bool invokeUpdate = false);
	//void bindDefault(GLuint shader, GLuint shaderLocation, TEXTURE_FALLBACK fallback);
	void bindDefaultOnly(TEXTURE_FALLBACK fallback);

	//void bind(TextureInfo &ti, GLuint shader, GLuint shaderLocation);

	static TextureManager* gTm;
};


#endif