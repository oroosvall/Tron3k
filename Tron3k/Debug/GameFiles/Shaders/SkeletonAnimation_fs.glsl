#version 430
layout(location = 0) in vec3 bonePosition; 
layout(location = 1) in vec2 boneUV;
layout(location = 2) in vec3 boneNormal;
layout(location = 3) in vec3 boneTagent;

uniform sampler2D textureSample;
uniform sampler2D normalSample;
uniform sampler2D glowSpecSample;

uniform vec3 dynamicGlowColor;
uniform float staticGlowIntensity;
uniform float trail;

layout (location = 1) out vec4 WorldPosOut;   
layout (location = 2) out vec4 DiffuseOut;     
layout (location = 3) out vec4 NormalOut;     
layout (location = 4) out vec4 GlowMap;

vec4 normalMap;
vec4 glowSpec;

vec4 CalcBumpedNormal()
{
	vec3 tan = normalize(boneTagent);
	tan = normalize(tan - dot(tan, boneNormal) * boneNormal);
	vec3 bitangent = cross(tan, boneNormal);
	normalMap = texture(normalSample, vec2(boneUV.s, 1-boneUV.t));
	vec4 newnormal = (2.0 * normalMap) - vec4(1.0, 1.0, 1.0, 0.0);
	mat3 TBN = mat3(tan, bitangent, boneNormal);
	newnormal.xyz = TBN * newnormal.xyz;
	newnormal.xyz = normalize(newnormal.xyz);
	return newnormal;
}

void main()
{
	WorldPosOut = vec4(bonePosition, 1.0);					
	DiffuseOut = texture(textureSample, vec2(boneUV.x, 1-boneUV.y));	
	NormalOut = CalcBumpedNormal();
	glowSpec = texture(glowSpecSample, vec2(boneUV.x, 1-boneUV.y));
	NormalOut.w = glowSpec.w;
	GlowMap = glowSpec * staticGlowIntensity + vec4((1.0 - normalMap.w) * dynamicGlowColor, 0);
	GlowMap.w = trail;
}