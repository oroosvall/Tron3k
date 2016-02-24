#version 410

layout(location = 0) in vec2 uv_frag;

layout(location = 1) in vec3 wPos;
layout(location = 2) in vec3 normal;

uniform sampler2D tex;

layout (location = 4) out vec4 GlowMap;

void main () 
{
	vec4 color = texture(tex, uv_frag);
	GlowMap = color;
}