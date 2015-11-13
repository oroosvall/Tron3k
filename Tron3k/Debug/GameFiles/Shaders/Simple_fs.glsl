#version 410
layout(location = 0) in vec2 UV;

//uniform sampler2D texture;
out vec4 fragment_color;

void main () 
{
	fragment_color = vec4( UV.x, UV.y, 1, 1);//texture(texture, vec2(UVs.s, 1-UVs.t)).xyz;
}