#ifndef GPUMEMORYLEAKCHECKER_H
#define GPUMEMORYLEAKCHECKER_H

#include <GL\glew.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

struct GLBuffer
{
	GLuint bufferID;
	string file;
	int line;
	bool free;
};

extern vector<GLBuffer> genBufferChecks;
extern vector<GLBuffer> genVertexArrayChecks;
extern vector<GLBuffer> genTextureCheck;

void glGenBuffers_D(GLsizei count, GLuint* id, char* file, int line);
void glGenVertexArray_D(GLsizei count, GLuint* id, char* file, int line);
void glGenTexture_D(GLsizei count, GLuint* id, char* file, int line);

void glDeleteBuffers_D(GLsizei count, GLuint* id);
void glDeleteVertexArray_D(GLsizei count, GLuint* id);
void glDeleteTexture_D(GLsizei count, GLuint* id);

void reportGPULeaks();

#ifndef REDEFCALLS

#undef glGenBuffers
#undef glGenVertexArrays
#undef glGenTextures

#undef glDeleteBuffers
#undef glDeleteVertexArrays
#undef glDeleteTextures

#define glGenBuffers(n,i)			glGenBuffers_D(n,i, __FILE__, __LINE__)
#define glGenVertexArrays(n,i)		glGenVertexArray_D(n,i, __FILE__, __LINE__)
#define glGenTextures(n,i)			glGenTexture_D(n,i, __FILE__, __LINE__)

#define glDeleteBuffers(n,i)		glDeleteBuffers_D(n,i)
#define glDeleteVertexArrays(n,i)	glDeleteVertexArray_D(n,i)
#define glDeleteTextures(n,i)		glDeleteTexture_D(n,i)
#endif

#endif