#version 410
layout (location = 0) in vec3 Position;                                                                  
layout (location = 1) in vec2 UV;                                                                  
layout (location = 2) in vec3 Normal;    
layout (location = 3) in vec3 Tangent;

uniform sampler2D textureSample;
uniform sampler2D normalSample;

layout (location = 1) out vec3 WorldPosOut;   
layout (location = 2) out vec3 DiffuseOut;     
layout (location = 3) out vec3 NormalOut;     
layout (location = 4) out vec3 TexCoordOut;

vec3 CalcBumpedNormal()
 {
  vec3 tan = normalize(Tangent);
  tan = normalize(tan - dot(tan, Normal) * Normal);
  vec3 bitangent = cross(tan, Normal);
  vec3 normalMap = texture(normalSample, vec2(UV.s, 1-UV.t)).xyz;
  normalMap = (2.0 * normalMap) - vec3(1.0, 1.0, 1.0);
  vec3 newnormal;
  mat3 TBN = mat3(tan, bitangent, Normal);
  newnormal = TBN * normalMap;
  newnormal = normalize(newnormal);
  return newnormal;
 }

void main()									
{			
	WorldPosOut	= Position;					
	DiffuseOut	= texture(textureSample, vec2(UV.x, 1-UV.y)).xyz;	
	NormalOut = CalcBumpedNormal();					
	TexCoordOut = vec3(UV, 0.0);				
}