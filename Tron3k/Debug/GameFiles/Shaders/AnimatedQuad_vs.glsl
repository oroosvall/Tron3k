#version 410
layout(location = 0) in vec3 vertex_pos;

void main()
{
	gl_Position =  vec4(vertex_position, 1);
}