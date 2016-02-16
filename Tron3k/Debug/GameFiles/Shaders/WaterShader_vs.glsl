#version 410
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 vertex_uv;

out vec2 uv;
out vec2 uv2;

uniform mat4 MVP;
uniform float anim;

void main()
{
	//Create
	gl_Position = MVP * vec4(pos, 1);
	uv = vertex_uv;
	uv2 = vertex_uv;
	
	//Animate
	uv.x = uv.x + anim/10;
	uv.y = uv.y - anim;
	uv2.x = uv2.x - anim/2;
}