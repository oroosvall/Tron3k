#version 410
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 UV;

layout(location = 0) out vec2 UVCord;

uniform mat4 worldMat;
uniform mat4 view;

uniform vec3 offset;

void main () 
{
	UVCord = UV;
	gl_Position =  view * vec4((vec4(vertex_position + offset, 1.0f) * worldMat).xyz, 1.0f);
}