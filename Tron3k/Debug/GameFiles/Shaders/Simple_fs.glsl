#version 410
layout(location = 0) in vec2 UV;

uniform sampler2D textureIN;
out vec4 fragment_color;

void main () 
{
	fragment_color = texture(textureIN, vec2(UV.s, 1-UV.t));
}