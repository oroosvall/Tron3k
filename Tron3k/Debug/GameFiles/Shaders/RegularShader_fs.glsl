#version 420
layout (location = 0) in vec3 Position;                                                                  
layout (location = 1) in vec2 UV;                                                                  
layout (location = 2) in mat3 TBN;

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
vec4 glowspec;
float alpha;

layout(early_fragment_tests) in;

vec4 CalcBumpedNormal()
 {
  normalMap = texture(normalSample, vec2(UV.s, 1-UV.t));
  alpha = normalMap.w;
  vec4 newnormal = (2.0 * normalMap) - vec4(1.0, 1.0, 1.0, 0.0);
  newnormal.xyz = TBN * newnormal.xyz;
  newnormal.xyz = normalize(newnormal.xyz);
  return newnormal;
 }

void main()									
{			
	WorldPosOut	= vec4(Position, 1.0);					
	DiffuseOut	= texture(textureSample, vec2(UV.x, 1-UV.y));	
	NormalOut = CalcBumpedNormal();
	glowspec = texture(glowSpecSample, vec2(UV.x, 1-UV.y));
	NormalOut.w = glowspec.w;
	GlowMap = glowspec * staticGlowIntensity + vec4((1.0f - alpha) * dynamicGlowColor, 0);
	GlowMap = clamp(GlowMap, vec4(0.0f), vec4(1.0f));
	GlowMap.w = trail * dot(vec3(1.0f), GlowMap.rgb);
}