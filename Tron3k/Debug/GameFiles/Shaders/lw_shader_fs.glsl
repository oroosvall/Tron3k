#version 410                                                          
layout (location = 0) in vec2 UV;         

uniform sampler2D texsample;

layout (location = 1) out vec4 WorldPosOut;   
layout (location = 2) out vec4 DiffuseOut;     
layout (location = 3) out vec4 NormalOut;     
layout (location = 4) out vec4 GlowMap;

void main()
{
	WorldPosOut	= vec4(0);				
	DiffuseOut	= vec4(0);	
	NormalOut = vec4(0);
	GlowMap += texture(texsample, vec2(UV.x, 1-UV.y));
}