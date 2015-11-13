#version 430

layout(location = 0) in vec2 UV;

uniform sampler2D textureSample;
out vec4 fragment_color;

void main () 
{
	fragment_color = texture(textureSample, vec2(UV.s, UV.t));
}