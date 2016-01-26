#version 410
layout(location = 0) in vec2 UV;

uniform sampler2D texture;
out vec4 fragment_color;

void main () 
{
	fragment_color = texture(texture, vec2(UV.s, 1-UV.t));
}