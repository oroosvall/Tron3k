#version 410
layout(location = 0) in vec3 bonePos;
layout(location = 1) in vec2 boneUV;
layout(location = 2) in vec3 boneNormal;
layout(location = 3) in vec3 boneTagent;
layout(location = 4) in vec4 boneIndices;
layout(location = 5) in vec4 skinWeights;

uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;

uniform float delta;

struct animationMatrices
{
	mat4 bMatrix;
};

layout (std140) uniform boneMatrices
{
	animationMatrices inverseBindpose[100];	
};	


layout(location = 0) out vec3 bonePos0;
layout(location = 1) out vec2 boneUV0;
layout(location = 2) out vec3 boneNormal0;
layout(location = 3) out vec3 boneTagent0;

void main()
{

	mat4 m1 = inverseBindpose[int(boneIndices.x)].bMatrix;
	mat4 m2 = inverseBindpose[int(boneIndices.y)].bMatrix;
	mat4 m3 = inverseBindpose[int(boneIndices.z)].bMatrix;
	mat4 m4 = inverseBindpose[int(boneIndices.w)].bMatrix;

	bonePos0 = 	((vec4(bonePos, 1.0f) * m1) * skinWeights.x).xyz;
	boneNormal0 = ((vec4(boneNormal, 1.0f) * m1) * skinWeights.x).xyz;
	boneTagent0 = ((vec4(boneTagent, 1.0f) * m1) * skinWeights.x).xyz;
	
	bonePos0 += ((vec4(bonePos, 1.0f) * m2) * skinWeights.y).xyz;
	boneNormal0 += ((vec4(boneNormal, 1.0f) * m2) * skinWeights.y).xyz;
	boneTagent0 += ((vec4(boneTagent, 1.0f) * m2) * skinWeights.y).xyz;
	
	bonePos0 += ((vec4(bonePos, 1.0f) * m3) * skinWeights.z).xyz;
	boneNormal0 += ((vec4(boneNormal, 1.0f) * m3) * skinWeights.z).xyz;
	boneTagent0 += ((vec4(boneTagent, 1.0f) * m3) * skinWeights.z).xyz;
	
	bonePos0 += ((vec4(bonePos, 1.0f) * m4) * skinWeights.w).xyz;
	boneNormal0 += ((vec4(boneNormal, 1.0f) * m4) * skinWeights.w).xyz;
	boneTagent0 += ((vec4(boneTagent, 1.0f) * m4) * skinWeights.w).xyz;

	bonePos0 = (vec4(bonePos0, 1.0f) * WorldMatrix).xyz;
	boneNormal0 = (vec4(boneNormal0, 0.0f) * WorldMatrix).xyz;
	boneTagent0 = (vec4(boneTagent0, 0.0f) * WorldMatrix).xyz;
	gl_Position = ViewProjMatrix * vec4(bonePos0.xyz, 1.0f);
	boneUV0 = boneUV;
}   