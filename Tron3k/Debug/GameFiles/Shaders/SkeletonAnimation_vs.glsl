#version 410

uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;
uniform mat4 inverseBindpose[100];

struct bones
{
	vec3 pos;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec4 boneIndices;
	vec4 skinWeights;
};

layout (std410, binding = 2) buffer boneArray
{
	particles data[];
};

layout(location = 0) out vec3 bonePos0;
layout(location = 1) out vec2 boneUV0;
layout(location = 2) out vec3 boneNormal0;
layout(location = 3) out vec3 boneTagent0;

void main()
{
	bonePos0 = (vec4(data[gl_VertexID].pos, 1.0f) * WorldMatrix).xyz;
	gl_Position = ViewProjMatrix * vec4(bonePos0, 1.0);
	boneUV0 = data[gl_VertexID].uv;
	boneNormal0 = data[gl_VertexID].normal;
	boneTagent0 = data[gl_VertexID].tangent;
}