#version 410
in vec2 UV;

uniform sampler2D textureSample;

out vec4 fragment_color;

void main () 
{
	fragment_color = texture(textureSample, vec2(UV.x, 1- UV.y));
	//vec4(1.0f, 0.0f, 0.0f, 1.0f) + texture(textureSample, 1 - UV) * 0.00000001f;
}