#version 410
layout (location = 0) in vec2 UV;

uniform int Use;
	
uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D UVcord;	
uniform sampler2D Depth;
	
vec4 Position0;
vec4 Diffuse0;
vec4 Normal0;
vec4 Depth0;

out vec4 fragment_color;

void main()
{
	if(Use == 0)
	{
		vec4 tempcolor = texture(Position, vec2(UV.x, UV.y));
		fragment_color = tempcolor * 0.25f;
	}
	else if(Use == 1)	 
		fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
	else if(Use == 2)	 
		fragment_color = texture(Normal, vec2(UV.x, UV.y));
	else if(Use == 3)	 
		fragment_color = texture(UVcord, vec2(UV.x, UV.y));
	else if(Use == 4)
	{
		float Depth = texture(Depth, vec2(UV.x, UV.y)).x;
		Depth = 1.0 - (1.0 - Depth) * 25.0; 
		fragment_color = vec4(Depth);
	}
	else if(Use == 5)
		fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
	else
		fragment_color = vec4(0,1,1,1);		
}