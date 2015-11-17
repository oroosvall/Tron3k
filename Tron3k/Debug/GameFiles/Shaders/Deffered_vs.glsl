#version 410
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUV;

layout (location = 0) out vec2 UVOut;  

void main()
{
	gl_Position = vec4(VertexPos ,1.0);
	UVOut = VertexUV;
}