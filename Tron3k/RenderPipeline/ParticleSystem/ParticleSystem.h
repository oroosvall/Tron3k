#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <glm/glm.hpp>
#include <random>
#include "../Utils/GPUMemoryLeakChecker.h"
#include "ParticleStructs.h"

class ParticleSystem
{
public:
	void Initialize(glm::vec3, ParticleSystemData, GLuint*, ParticleShaderLocations*);
	void Update(float);
	void Draw();

	void Release();

	void setSpawnPos(glm::vec3 pos);

	void startDying();

	glm::vec2 m_size;
	glm::vec3 m_color;

	GLuint m_texture;
	int m_glow;

	bool m_alive;
	int m_counter;
	int m_scaleDir;

	bool created;

private:
	ParticleSystemData m_data;
	//std::vector<Particle> m_vertices;
	Particle* m_vertices;

	ParticleShaderLocations* m_loc;

	glm::vec3 m_pos;

	float m_currentEmission;

	float timer;

	//Uniforms
	GLuint* m_program;	//shaders
	GLuint m_vbo;
	GLuint m_vao;
};
#endif