#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <glm/glm.hpp>
#include "../Utils/GPUMemoryLeakChecker.h"
#include "ParticleStructs.h"

class ParticleSystem
{
public:
	void Initialize(glm::vec3, ParticleSystemData*, GLuint*);
	void Update(float);
	void Draw();

	void Release();

	glm::vec2 m_size;

private:
	ParticleSystemData* m_data;
	std::vector<Particle> m_vertices;

	glm::vec3 m_pos;

	float m_currentEmission;

	//Uniforms
	GLuint* m_program;	//shaders
	GLuint m_vbo;
	GLuint m_vao;

	GLuint lifetime;
	GLuint deltaTime;
	GLuint force;
	GLuint drag;
	GLuint gravity;
	GLuint emission;
	GLuint continuous;
	GLuint omni;		
	GLuint initialPos;
	GLuint bufferLocation;
};
#endif