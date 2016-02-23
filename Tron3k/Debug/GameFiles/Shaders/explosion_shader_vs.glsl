#version 420
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUV;
//layout(location = 2) in vec3 VertexNormal;
//layout(location = 3) in vec3 TangentIn;

uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;

layout(location = 0) out vec2 UV;

void main()
{
	gl_Position = ViewProjMatrix * (vec4(VertexPos, 1.0) * WorldMatrix);
	UV = VertexUV;
}