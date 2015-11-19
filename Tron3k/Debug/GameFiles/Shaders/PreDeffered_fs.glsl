#version 410
layout (location = 0) in vec3 Position;                                                                  
layout (location = 1) in vec2 UV;                                                                  
layout (location = 2) in vec3 Normal;    

uniform sampler2D textureSample;

layout (location = 1) out vec3 WorldPosOut;   
layout (location = 2) out vec3 DiffuseOut;     
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;


void main()									
{			
	WorldPosOut	= Position;					
	DiffuseOut	= texture(textureSample, vec2(UV.x, 1-UV.y)).xyz;	
	NormalOut = Normal;					
	TexCoordOut = vec3(UV, 0.0);				
}