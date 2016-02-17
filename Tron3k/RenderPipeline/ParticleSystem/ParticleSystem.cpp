#include "ParticleSystem.h"

void ParticleSystem::Initialize(glm::vec3 pos, ParticleSystemData* ps, GLuint* program)
{
	m_program = program;
	m_data = ps;

	m_size = glm::vec2(m_data->width, m_data->height);

	for (int i = 0; i < m_data->maxparticles; i++)
	{
		Particle p;
		//p.pos = glm::vec4(0, 0, 0, 0);
		p.pos = glm::vec4(pos, 0);
		p.dir = glm::vec4(0, 0, 0, -1.0f);
		glm::vec3 dir = m_data->dir;
		if (m_data->omni)
		{
			float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			dir = glm::normalize(glm::vec3(x-1.0f, y - 1.0f, z - 1.0f));
		}
		p.iDir = glm::vec4(dir, 0);
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
	glProgramUniform1i(*m_program, emission, m_data->emission);
	glProgramUniform1f(*m_program, continuous, m_data->continuous);
	glProgramUniform1f(*m_program, omni, m_data->omni);
	glProgramUniform3f(*m_program, initialPos, pos.x, pos.y, pos.z);
	
	m_pos = pos;

}


void ParticleSystem::Update(float dT)
{
	int emit = 123;
	if (m_currentEmission < 0.0f)
	{
		m_currentEmission = m_data->emission;
		emit = -1;
	}
	else
	{
		m_currentEmission -= dT;
	}

	if (m_data->emission == 0.0f)
	{
		emit = 0;
	}

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
	glProgramUniform1i(*m_program, emission, m_data->emission);
	glProgramUniform1f(*m_program, continuous, m_data->continuous);
	glProgramUniform1f(*m_program, omni, m_data->omni);
	glProgramUniform3f(*m_program, initialPos, m_pos.x, m_pos.y, m_pos.z);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vbo);

	glProgramUniform1f(*m_program, deltaTime, dT);
	glProgramUniform1i(*m_program, emission, emit);

	glDispatchCompute(1, 1, 1);
}

void ParticleSystem::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindVertexArray(m_vao);

	glDrawArrays(GL_POINTS, 0, m_vertices.size());
}

void ParticleSystem::Release()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}