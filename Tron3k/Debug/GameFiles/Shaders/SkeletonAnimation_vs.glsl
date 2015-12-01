#version 410
layout(location = 0) in vec3 bonePos;
layout(location = 1) in vec2 boneUV;
layout(location = 2) in vec3 boneNormal;
layout(location = 3) in vec3 boneTagent;

uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;
uniform mat4 BoneMatrices[100];

layout(location = 0) out vec3 bonePos0;
layout(location = 1) out vec2 boneUV0;
layout(location = 2) out vec3 boneNormal0;
layout(location = 3) out vec3 boneTagent0;

void main()
{
	bonePos0 = (vec4(bonePos, 1.0f) * WorldMatrix).xyz;
	gl_Position = ViewProjMatrix * vec4(bonePos0, 1.0);
	boneUV0 = boneUV;
	boneNormal0 = boneNormal;
	boneTagent0 = boneTagent;
}