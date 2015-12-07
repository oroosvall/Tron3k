#version 410
layout(location = 0) in vec3 bonePos;
layout(location = 1) in vec2 boneUV;
layout(location = 2) in vec3 boneNormal;
layout(location = 3) in vec3 boneTagent;
layout(location = 4) in vec4 boneIndices;
layout(location = 5) in vec4 skinWeights;

uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;

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

	bonePos0 = ((vec4(bonePos, 1.0f) * inverseBindpose[int(boneIndices.x)].bMatrix) * skinWeights.x).xyz;
	//boneNormal0 = ((vec4(boneNormal, 1.0f) * inverseBindpose[boneIndices.x].bMatrix) * 1.0f).xyz;
	//boneTagent0 = ((vec4(boneTagent, 1.0f) * inverseBindpose[boneIndices.x].bMatrix) * 1.0f).xyz;
	
	bonePos0 += ((vec4(bonePos0, 1.0f) * inverseBindpose[int(boneIndices.y)].bMatrix) * skinWeights.y).xyz;
	//boneNormal0 += ((vec4(boneNormal, 1.0f) * inverseBindpose[boneIndices.y].bMatrix) * 1.0f).xyz;
	//boneTagent0 += ((vec4(boneTagent, 1.0f) * inverseBindpose[boneIndices.y].bMatrix) * 1.0f).xyz;
	
	bonePos0 += ((vec4(bonePos0, 1.0f) * inverseBindpose[int(boneIndices.z)].bMatrix) * skinWeights.z).xyz;
	//boneNormal0 += ((vec4(boneNormal, 1.0f) * inverseBindpose[boneIndices.z].bMatrix) * 1.0f).xyz;
	//boneTagent0 += ((vec4(boneTagent, 1.0f) * inverseBindpose[boneIndices.z].bMatrix) * 1.0f).xyz;
	
	bonePos0 += ((vec4(bonePos0, 1.0f) * inverseBindpose[int(boneIndices.w)].bMatrix) * skinWeights.w).xyz;
	//boneNormal0 += ((vec4(boneNormal, 1.0f) * inverseBindpose[boneIndices.w].bMatrix) * 1.0f).xyz;
	//boneTagent0 += ((vec4(boneTagent, 1.0f) * inverseBindpose[boneIndices.w].bMatrix) * 1.0f).xyz;

	bonePos0 = (vec4(bonePos0, 1.0f) * WorldMatrix).xyz;
	gl_Position = ViewProjMatrix * vec4(bonePos0.xyz, 1.0f);
	boneUV0 = boneUV;
	
	boneNormal0 = boneNormal;
	boneTagent0 = boneTagent;
}   