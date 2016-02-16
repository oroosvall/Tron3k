#version 430
#define WORKGROUP_SIZE 32
#define PIXEL_PER_THREAD 4

struct Particle
{
    vec4 pos;
	//float intensity; == dir.w
	vec4 dir; // w == 0.0f
	vec4 initialDir; // w == 0.0f
};

layout(std430, binding = 0) buffer ParticleBuffer
{
    Particle l[];
};

uniform float lifetime; // when reset full intensity
uniform float deltaTime;
uniform float force;
uniform float drag;
uniform float gravity;
uniform float emission;

uniform bool continuous;
uniform bool omni;

uniform vec3 initialPos; // w == 0.0f

shared uint indexCounter;

layout (local_size_x = WORKGROUP_SIZE, local_size_y = WORKGROUP_SIZE) in;
void main()
{

	// for each move it, reduce intensity, (when render check intensity)
	// if continuous reset particle
	
	
}