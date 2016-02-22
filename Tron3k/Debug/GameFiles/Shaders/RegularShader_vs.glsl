#version 420
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec3 TangentIn;

uniform mat4 WorldMatrix[100];
uniform mat4 ViewMatrix;
uniform mat4 ViewProjMatrix;

layout(location = 0) out vec3 PositionOut;
layout(location = 1) out vec2 UVOut;
layout(location = 2) out vec3 NormalOut;
layout(location = 3) out vec3 TangentOut;

in int gl_InstanceID ;

void main()
{
	PositionOut = (vec4(VertexPos, 1.0) * WorldMatrix[gl_InstanceID]).xyz;
	gl_Position = ViewProjMatrix * vec4(PositionOut, 1.0);
	UVOut = VertexUV;
	NormalOut = (vec4(VertexNormal, 0.0) * WorldMatrix[gl_InstanceID]).xyz;
	TangentOut = (vec4(TangentIn, 0.0) * WorldMatrix[gl_InstanceID]).xyz;
}