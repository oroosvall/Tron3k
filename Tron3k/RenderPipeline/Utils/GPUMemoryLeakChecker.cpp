#define REDEFCALLS

#include "GPUMemoryLeakChecker.h"
#include <iostream>
#include <Windows.h>

#include <map>

#include <sstream>

using std::cout;
using std::endl;

vector<GLBuffer> genBufferChecks;
vector<GLBuffer> genVertexArrayChecks;
vector<GLBuffer> genTextureCheck;

unsigned int drawCount = 0;
unsigned int primitiveCount = 0;

unsigned int genBufferPeak = 0;
unsigned int genVaoPeak = 0;
unsigned int genTexturePeak = 0;

unsigned int memusageTex = 0;
unsigned int memusageMesh = 0;
unsigned int memusageT = 0;

unsigned int textureBinds = 0;
unsigned int bufferBinds = 0;

unsigned int shaderBinds = 0;

unsigned int stateChange = 0;

 unsigned int texManBinds;
 unsigned int illegalBinds;

GLuint lastProgram = 0;
GLenum currentActiveTexture = GL_TEXTURE0;

GLenum arrayBufferLast = 0;
GLenum elementBufferLast = 0;

GLuint textureBindMap[5];

GLuint bufferBindMap[5];

unsigned int memoryLimit = 256 * 1024 * 1024; // 256 MB limit :)

void glGenBuffers_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;
	
	glGenBuffers(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;

	//genBufferChecks.push_back(buf);
	genBufferPeak++;
}

void glGenVertexArray_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;

	glGenVertexArrays(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;
	
	//genVertexArrayChecks.push_back(buf);
	genVaoPeak++;
}

void glGenTexture_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;

	glGenTextures(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;

	genTextureCheck.push_back(buf);
	genTexturePeak++;
}

void glDeleteBuffers_D(GLsizei n, GLuint* id)
{
	//for (size_t i = 0; i < genBufferChecks.size(); i++)
	//{
	//	if (genBufferChecks[i].bufferID == *id)
	//	{
			GLint oldBufferSize = 0;
			glBindBuffer(GL_ARRAY_BUFFER, *id);
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &oldBufferSize);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			memusageMesh -= oldBufferSize;
			memusageT -= oldBufferSize;
			glDeleteBuffers(n, id);
	//		genBufferChecks.erase(genBufferChecks.begin()+i);
	//		break;
	//	}
	//}
	genBufferPeak--;
}

void glDeleteVertexArray_D(GLsizei n, GLuint* id)
{
	//for (size_t i = 0; i < genVertexArrayChecks.size(); i++)
	//{
	//	if (genVertexArrayChecks[i].bufferID == *id)
	//	{
			glDeleteVertexArrays(n, id);
	//		genVertexArrayChecks.erase(genVertexArrayChecks.begin() + i);
	//		break;
	//	}
	//}
	genVaoPeak--;
}

void glDeleteTexture_D(GLsizei n, GLuint* id)
{
	for (size_t i = 0; i < genTextureCheck.size(); i++)
	{
		if (genTextureCheck[i].bufferID == *id)
		{


			int oldW = 0, oldH = 0;
			int oldSize = 0;

			int compressed = 0;

			int internalComponentSize = 0;

			GLenum target = GL_TEXTURE_2D;
			GLint level = 0;
			glBindTexture(target, *id);
			glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED, (GLint*)&compressed);
			if (compressed)
				glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &oldSize);
			else
			{
				glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &oldW);
				glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &oldH);

				glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
				int temp;
				glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
				internalComponentSize += temp;
				glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
				internalComponentSize += temp;
				glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
				internalComponentSize += temp;
				glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
				internalComponentSize += temp;

				internalComponentSize /= 8;

				oldSize = oldW * oldH * internalComponentSize;

			}

			memusageTex -= oldSize;
			memusageT -= oldSize;

			//printf("Size freed %d\n", oldSize);

			glDeleteTextures(n, id);
			genTextureCheck.erase(genTextureCheck.begin() + i);
			break;
		}
	}
	genTexturePeak--;
}

void glDrawElements_D(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
	drawCount++;
	glDrawElements(mode, count, type, indices);

	if(mode == GL_TRIANGLES)
		primitiveCount += (count / 3);
}

void glDrawElementsInstanced_D(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount)
{
	drawCount++;
	glDrawElementsInstanced(mode, count, type, indices, primcount);
	if (mode == GL_TRIANGLES)
		primitiveCount += ((count / 3) * primcount);
}

void glDrawArrays_D(GLenum mode, GLint first, GLsizei count)
{
	drawCount++;
	glDrawArrays(mode, first, count);
	if (mode == GL_TRIANGLES)
		primitiveCount += (count / 3);
	else if(mode == GL_TRIANGLE_STRIP)
		primitiveCount += count;

}

void glBufferData_D(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage)
{
	GLint oldBufferSize = 0;
	glGetBufferParameteriv(target, GL_BUFFER_SIZE, &oldBufferSize);
	glBufferData(target, size, data, usage);

	memusageMesh += (size - oldBufferSize);
	memusageT += (size - oldBufferSize);

}

void glActiveTexture_D(GLenum texture)
{
	if (texture != currentActiveTexture)
	{
		currentActiveTexture = texture;
		glActiveTexture(texture);
	}
}

void glBindTexture_D(GLenum target, GLuint texture,const char* funcName)
{
	//if (!(funcName == "bind" || funcName == "bindDefault" || funcName == "bindDefaultOnly" || funcName == "bindTextureOnly"))
	//{
	//	//printf("I blame you for broken stuff %s\n", funcName);
	//	illegalBinds++;
	//}
	//else
	//{
	//	texManBinds++;
	//}
	//if (textureBindMap[currentActiveTexture - GL_TEXTURE0] != texture)
	//{
		textureBinds++;
		glBindTexture(target, texture);
	//	textureBindMap[currentActiveTexture - GL_TEXTURE0] = texture;
	//}
}

void glBindBuffer_D(GLenum target, GLuint buffer)
{
	if (bufferBindMap[target - GL_ARRAY_BUFFER] != buffer)
	{
		bufferBinds++;
		glBindBuffer(target, buffer);
		bufferBindMap[target - GL_ARRAY_BUFFER] = buffer;
	}
}

void glTexImage2D_D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data)
{
	int oldW = 0, oldH = 0;
	int oldSize = 0;

	int newW = 0, newH = 0;
	int newSize = 0;

	int compressed = 0;

	int internalComponentSize = 0;

	glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED, (GLint*)&compressed);
	if (compressed)
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &oldSize);
	else
	{
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &oldW);
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &oldH);

		glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
		int temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
		internalComponentSize += temp;

		internalComponentSize /= 8;

		oldSize = oldW * oldH * internalComponentSize;

	}

	unsigned int asumedSize = width * height * 4;

	// check if we can allocate that memory, if not I'll give you a 1x1 RED pixel only, now take care of the resources
	if(asumedSize + memusageT < memoryLimit)
		glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
	else
	{
		glTexImage2D(target, level, GL_R8, 1, 1, border, GL_RGBA, type, data);
		printf("Failed to allocate texture, Out of memory!");
	}

	glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED, (GLint*)&compressed);
	if (compressed)
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &newSize);
	else
	{
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &newW);
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &newH);

		glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
		int temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
		internalComponentSize += temp;

		internalComponentSize /= 8;

		newSize = newW * newH * internalComponentSize;
	}

	printf("UPLOAD: Texture size %d bytes\n", newSize);

	memusageTex += (newSize - oldSize);
	memusageT += (newSize - oldSize);

}

void glCompressedTexImage2D_D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data)
{

	int oldW = 0, oldH = 0;
	int oldSize = 0;

	int newW = 0, newH = 0;
	int newSize = 0;

	int compressed = 0;

	int internalComponentSize = 0;

	glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED, (GLint*)&compressed);
	if (compressed)
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &oldSize);
	else
	{
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &oldW);
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &oldH);

		glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
		int temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
		internalComponentSize += temp;

		internalComponentSize /= 8;

		oldSize = oldW * oldH * internalComponentSize;

	}
	// check if we can allocate that memory, if not I'll give you a 1x1 RED pixel only, now take care of the resources
	if(imageSize + memusageT < memoryLimit && imageSize > 0)
		glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
	else
	{
		glTexImage2D(target, level, GL_R8, 1, 1, border, GL_RGBA, GL_BYTE, data);
		if(imageSize>0)
			printf("Failed to allocate texture, Out of memory!\n");
		else
			printf("Failed to allocate texture, invalid texture size");
	}

	glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED, (GLint*)&compressed);
	if (compressed)
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &newSize);
	else
	{
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &newW);
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &newH);

		glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
		int temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
		internalComponentSize += temp;

		internalComponentSize /= 8;

		newSize = newW * newH * internalComponentSize;

	}

	memusageTex += (newSize - oldSize);
	memusageT += (newSize - oldSize);

}

void glUseProgram_D(GLuint program)
{
	if (program != lastProgram)
	{
		shaderBinds++;
		glUseProgram(program);
	}
	lastProgram = program;
}

void reportGPULeaks()
{
	bool found = false;
	for (size_t i = 0; i < genBufferChecks.size(); i++)
	{
		if (genBufferChecks[i].free == false)
		{
			cout << "glGenBuffers: " << genBufferChecks[i].bufferID << "\n" << genBufferChecks[i].file << " : " << genBufferChecks[i].line << endl;
			std::stringstream bla;
			bla << "glGenBuffers: " << genBufferChecks[i].bufferID << "\n" << genBufferChecks[i].file << " : " << genBufferChecks[i].line << endl;
			OutputDebugStringA(bla.str().c_str());
			found = true;
		}
	}

	for (size_t i = 0; i < genVertexArrayChecks.size(); i++)
	{
		if (genVertexArrayChecks[i].free == false)
		{
			cout << "glGenVertexArrays: " << genVertexArrayChecks[i].bufferID << "\n" << genVertexArrayChecks[i].file << " : " << genVertexArrayChecks[i].line << endl;
			std::stringstream bla;
			bla << "glGenVertexArrays: " << genVertexArrayChecks[i].bufferID << "\n" << genVertexArrayChecks[i].file << " : " << genVertexArrayChecks[i].line << endl;
			OutputDebugStringA(bla.str().c_str());
			found = true;
		}
	}

	for (size_t i = 0; i < genTextureCheck.size(); i++)
	{
		if (genTextureCheck[i].free == false)
		{
			cout << "glGenTextures: " << genTextureCheck[i].bufferID << "\n" << genTextureCheck[i].file << " : " << genTextureCheck[i].line << endl;
			std::stringstream bla;
			bla << "glGenTextures: " << genTextureCheck[i].bufferID << "\n" << genTextureCheck[i].file << " : " << genTextureCheck[i].line << endl;
			OutputDebugStringA(bla.str().c_str());
			found = true;
		}
	}

	if (found)
	{
		//breakLaptopBySlammingItClosedCauseIWasWatchingPornAtWork();
		// if break here u have gpu mem leaks, press break and check console
		//DebugBreak();
	}

}

void glEnable_D(GLenum state)
{
	stateChange++;
	glEnable(state);
}

void glDisable_D(GLenum state)
{
	stateChange++;
	glDisable(state);
}

#undef glGenBuffers
#undef glGenVertexArrays
#undef glGenTextures

#undef glDeleteBuffers
#undef glDeleteVertexArrays
#undef glDeleteTextures

#undef glDrawElements
#undef glDrawArrays_D

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

#define glDrawArrays(m, f, c)		glDrawArrays_D(m, f, c)

#define glBufferData(target, size, data, usage) glBufferData_D(target, size, data, usage)

#define glActiveTexture(texture) glActiveTexture_D(texture)
#define glBindTexture(target, texture) glBindTexture_D(target, texture, __func__)

#define glBindBuffer(target, buffer)	glBindBuffer_D(target, buffer) 

#define glTexImage2D(target, level, internalFormat, width, height, border, format, type, data) glTexImage2D_D(target, level, internalFormat, width, height, border, format, type, data)

#define glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data) glCompressedTexImage2D_D(target, level, internalformat, width, height, border, imageSize, data)

#define glUseProgram(program)	glUseProgram_D(program)

#define glEnable(state)		glEnable_D(state)
#define glDisable(state)	glDisable_D(state)