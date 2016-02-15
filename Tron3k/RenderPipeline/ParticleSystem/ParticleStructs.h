#include <glm/glm.hpp>

struct Particle
{
	glm::vec4 pos;
	glm::vec4 dir; //NOTE: .w holds intensity!
	glm::vec4 iDir;
};

struct ParticleSystemData
{
	glm::vec3 dir;
	float width;
	float height;
	int maxparticles;
	float lifetime;
	float emission;
	float force;
	float drag;
	float gravity;
	bool continuous;
	bool omni;
};