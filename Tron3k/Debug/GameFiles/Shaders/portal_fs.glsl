#version 410

layout(location = 0) in float dist;

layout (location = 1) out vec4 WorldPosOut;   
layout (location = 2) out vec4 DiffuseOut;     
layout (location = 3) out vec4 NormalOut;     
layout (location = 4) out vec4 GlowMap;

void main () 
{
	float maxlen = 100;
	float transp = dist;
	
	if(transp > maxlen * 0.9)
		transp = 0.1;
	else
		transp =  1 - (transp / maxlen);
  
	WorldPosOut	= vec4(0);
	DiffuseOut = vec4((1 - transp) / 1.5 , 0 , transp / 2, 0.25 + transp / 4);	
	NormalOut = vec4(0);
	GlowMap = vec4(0);
}