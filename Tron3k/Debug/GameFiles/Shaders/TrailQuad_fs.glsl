#version 410
layout (location = 0) in vec2 uv;

uniform sampler2D tex;
uniform vec3 dgcolor;
    
layout (location = 4) out vec4 GlowMap;

void main () 
{
		GlowMap = texture(tex, uv) * vec4(dgcolor, 1);
}