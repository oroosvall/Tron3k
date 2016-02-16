#include "ParticleSystem.h"

void ParticleSystem::Initialize(glm::vec3 pos, ParticleSystemData* ps, GLuint* program)
{
	m_program = program;
	m_data = ps;

	m_size = glm::vec2(m_data->width, m_data->height);

	for (int i = 0; i < m_data->maxparticles; i++)
	{
		Particle p;
		p.pos = glm::vec4(0, 0, 0, 0);
		p.dir = glm::vec4(0, 0, 0, 0);
		p.iDir = glm::vec4(m_data->dir, 0);
		m_vertices.push_back(p);
	}


	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
	//pos
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 3, BUFFER_OFFSET(0));
	//dir
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 3, BUFFER_OFFSET(sizeof(float) * 4));
	//iDir
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 3, BUFFER_OFFSET(sizeof(float) * 8));


	deltaTime = glGetUniformLocation(*m_program, "deltaTime");
	lifetime = glGetUniformLocation(*m_program, "lifetime");
	force = glGetUniformLocation(*m_program, "force");
	drag = glGetUniformLocation(*m_program, "drag");
	gravity = glGetUniformLocation(*m_program, "gravity");
	emission = glGetUniformLocation(*m_program, "emission");
	continuous = glGetUniformLocation(*m_program, "continuous");
	omni = glGetUniformLocation(*m_program, "omni");
	initialPos = glGetUniformLocation(*m_program, "initialPos");

	glProgramUniform1f(*m_program, lifetime, m_data->lifetime);
	glProgramUniform1f(*m_program, force, m_data->force);
	glProgramUniform1f(*m_program, drag, m_data->drag);
	glProgramUniform1f(*m_program, gravity, m_data->gravity);
	glProgramUniform1f(*m_program, emission, m_data->emission);
	glProgramUniform1f(*m_program, continuous, m_data->continuous);
	glProgramUniform1f(*m_program, omni, m_data->omni);
	glProgramUniform1f(*m_program, emission, m_data->emission);
	glProgramUniform3f(*m_program, initialPos, pos.x, pos.y, pos.z);
}


void ParticleSystem::Update(float dT)
{
	glProgramUniform1f(*m_program, deltaTime, dT);
	glDispatchCompute(1, 0, 0);
}

void ParticleSystem::Draw()
{
	glDrawArrays(GL_POINTS, 0, m_vertices.size());
}

void ParticleSystem::Release()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_vao);
}