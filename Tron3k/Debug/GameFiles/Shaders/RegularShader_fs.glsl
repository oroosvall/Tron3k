#version 410
layout (location = 0) in vec3 Position;                                                                  
layout (location = 1) in vec2 UV;                                                                  
layout (location = 2) in vec3 Normal;    
layout (location = 3) in vec3 Tangent;


uniform sampler2D textureSample;
uniform sampler2D normalSample;
uniform sampler2D glowSpecSample;

uniform vec3 dynamicGlowColor;
uniform float staticGlowIntensity;

layout (location = 1) out vec4 WorldPosOut;   
layout (location = 2) out vec4 DiffuseOut;     
layout (location = 3) out vec4 NormalOut;     
layout (location = 4) out vec4 GlowMap;

vec4 normalMap;

vec4 CalcBumpedNormal()
 {
  vec3 tan = normalize(Tangent);
  tan = normalize(tan - dot(tan, Normal) * Normal);
  vec3 bitangent = cross(tan, Normal);
  normalMap = texture(normalSample, vec2(UV.s, 1-UV.t));
  vec4 newnormal = (2.0 * normalMap) - vec4(1.0, 1.0, 1.0, 0.0);
  mat3 TBN = mat3(tan, bitangent, Normal);
  newnormal.xyz = TBN * newnormal.xyz;
  newnormal.xyz = normalize(newnormal.xyz);
  
  return newnormal;
 }

void main()									
{			
	WorldPosOut	= vec4(Position, 0.0);					
	DiffuseOut	= texture(textureSample, vec2(UV.x, 1-UV.y));	
	NormalOut = CalcBumpedNormal();
	GlowMap = texture(glowSpecSample, vec2(UV.x, UV.y)) * staticGlowIntensity;
	GlowMap +=  vec4((1.0 - normalMap.w) * dynamicGlowColor, 0);
}