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

extern unsigned int drawCount;
extern unsigned int primitiveCount;

extern unsigned int genBufferPeak;
extern unsigned int genVaoPeak;
extern unsigned int genTexturePeak;

extern unsigned int memusageTex;
extern unsigned int memusageMesh;
extern unsigned int memusageT;

extern unsigned int textureBinds;

extern unsigned int bufferBinds;

extern unsigned int shaderBinds;

extern unsigned int stateChange;

extern unsigned int texManBinds;
extern unsigned int illegalBinds;


#ifdef _DEBUG

void glGenBuffers_D(GLsizei count, GLuint* id, char* file, int line);
void glGenVertexArray_D(GLsizei count, GLuint* id, char* file, int line);
void glGenTexture_D(GLsizei count, GLuint* id, char* file, int line);

void glDeleteBuffers_D(GLsizei count, GLuint* id);
void glDeleteVertexArray_D(GLsizei count, GLuint* id);
void glDeleteTexture_D(GLsizei count, GLuint* id);

void glDrawElements_D(GLenum mode, GLsizei count, GLenum type, const void* indices);

void glDrawElementsInstanced_D(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount);

void glDrawArrays_D(GLenum mode, GLint first, GLsizei count);

void glBufferData_D(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);

void glActiveTexture_D(GLenum texture);
void glBindTexture_D(GLenum target, GLuint texture, const char* funcName);

void glBindBuffer_D(GLenum target, GLuint buffer);

void glTexImage2D_D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data);

void glCompressedTexImage2D_D(GLenum target, GLint level,	GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);

void glUseProgram_D(GLuint program);

void glEnable_D(GLenum state);

void glDisable_D(GLenum state);

void reportGPULeaks();

#ifndef REDEFCALLS

#undef glGenBuffers
#undef glGenVertexArrays
#undef glGenTextures

#undef glDeleteBuffers
#undef glDeleteVertexArrays
#undef glDeleteTextures
#undef glDrawElements

#undef glDrawArrays

#undef glBufferData
#undef glActiveTexture
#undef glBindTexture

#undef glBindBuffer

#undef glTexImage2D
#undef glCompressedTexImage2D

#undef glUseProgram

#undef glEnable
#undef glDisable

#undef glDrawElementsInstanced

#define glGenBuffers(n,i)			glGenBuffers_D(n,i, __FILE__, __LINE__)
#define glGenVertexArrays(n,i)		glGenVertexArray_D(n,i, __FILE__, __LINE__)
#define glGenTextures(n,i)			glGenTexture_D(n,i, __FILE__, __LINE__)

#define glDeleteBuffers(n,i)		glDeleteBuffers_D(n,i)
#define glDeleteVertexArrays(n,i)	glDeleteVertexArray_D(n,i)
#define glDeleteTextures(n,i)		glDeleteTexture_D(n,i)

#define glDrawElements(m, c, t ,i)		glDrawElements_D(m, c, t ,i)
#define glDrawElementsInstanced(mode, count, type, indices, primcount)	glDrawElementsInstanced_D(mode, count, type, indices, primcount)

#define glDrawArrays(m, f, c)			glDrawArrays_D(m, f, c)

#define glBufferData(target, size, data, usage) glBufferData_D(target, size, data, usage)

#define glActiveTexture(texture) glActiveTexture_D(texture)
#define glBindTexture(target, texture) glBindTexture_D(target, texture, __func__)

#define glBindBuffer(target, buffer)	glBindBuffer_D(target, buffer) 

#define glUseProgram(program)	glUseProgram_D(program)

#define glTexImage2D(target, level, internalFormat, width, height, border, format, type, data) glTexImage2D_D(target, level, internalFormat, width, height, border, format, type, data)

#define glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data) glCompressedTexImage2D_D(target, level, internalformat, width, height, border, imageSize, data)

#define glEnable(state)		glEnable_D(state)
#define glDisable(state)	glDisable_D(state)


#endif

#else

void reportGPULeaks();

#endif

#endif