#version 410
layout (location = 0) in vec2 UV;

uniform int Use;
	
uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D UVcord;	
uniform sampler2D Depth;

out vec4 fragment_color;

void main()
{
	//fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
	if(Use == 0)
	{
		float Depth = texture(Depth, vec2(UV.x, UV.y)).x;
		fragment_color = vec4(1.0 - (1.0 - Depth) * 25.0); 
		//fragment_color = vec4(Depth);
	}
	else if(Use == 1)
		fragment_color = texture(Position, vec2(UV.x, UV.y)) * 0.25;
	//fragment_color = tempcolor * 0.25f;
	else if(Use == 2)	 
		fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
	else if(Use == 3)	 
		fragment_color = texture(Normal, vec2(UV.x, UV.y));
	else if(Use == 4)	 
		//fragment_color = texture(UVcord, vec2(UV.x, UV.y));
		fragment_color = vec4(1.0f);
	else if(Use == 5)
		fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
	else
		fragment_color = vec4(0,1,1,1);		
}