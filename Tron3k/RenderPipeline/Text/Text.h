#ifndef TEXT_H
#define TEXT_H

#include <vector>
#include <glm\glm.hpp>
#include <string>

#include "../Utils/GPUMemoryLeakChecker.h"

using namespace std;
using namespace glm;

struct Float5
{
	float x, y, z;
	float u, v;
};

class Text
{

private:


	GLuint vbo;
	GLuint vao;

	GLuint texture;

	std::string text;
	int fontSize;

	vec2 screenPos;

	int quadCount;

	vec3 centerOffset;

	bool TextScale;

public:

	Text(std::string text, int fontsize, GLuint textureId, vec2 screenPos, bool scale = false);
	~Text();

	static int ScreenResWidth;
	static int ScreenResHeight;

	static int ScreenResWidthBase;
	static int ScreenResHeightBase;

	vec3 getOffset()
	{
		return centerOffset;
	}

	void setPos(vec2 screenPos);
	void setTexture(GLuint textureId);
	void setSize(int fontSize);
	void setText(std::string text);
	
	void fillBuffer();

	void draw();

};

#endif