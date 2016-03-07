#version 410

//layout (location = 0) in vec2 UV; 
layout(location = 1)out vec4 fragment_color;


uniform int pointlightID;
uniform vec3 eyepos;

uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D GlowMap;

uniform float universalInten;
uniform vec2 gScreenSize;


struct SpotLight
{
	vec3 Color;
	float DiffuseIntensity;
	vec3 Position;
	float AmbientIntensity;
	vec3 Direction;
	float Cutoff;
	vec4 attenuation;
};

layout (std140) uniform Light
{ 
	SpotLight lights[500];
};

vec3 Position0;
vec4 Normal0;
vec4 GlowSpec0;
vec4 Diffuse0;

float gSpecularPower = 200;
float gMatSpecularIntensity = 0.4;

void main()
{
	fragment_color = vec4(0);
	float size = lights[pointlightID].attenuation.w;
	
	vec2 UV = gl_FragCoord.xy / gScreenSize;
	
	Position0 = texture(Position, UV).xyz;
	
	vec3 LightDirection = Position0.xyz - lights[pointlightID].Position;
	float Distance = length(LightDirection);
	
	if(Distance < size)
	{
		LightDirection = normalize(LightDirection);
		Normal0 = texture(Normal, UV);	
		float DiffuseFactor = dot(Normal0.xyz, -LightDirection); 
		
		if (DiffuseFactor > 0) 
		{	
			//directional light
			fragment_color = vec4(lights[pointlightID].Color, 1.0f) * lights[pointlightID].DiffuseIntensity * DiffuseFactor;
			
			//specular
			vec4 specularAddetive = vec4(0);
		
			vec3 VertexToEye = normalize(eyepos - Position0);                             
			vec3 LightReflect = normalize(reflect(LightDirection, Normal0.xyz));                     
			float SpecularFactor = dot(VertexToEye, LightReflect);                              
			SpecularFactor = pow(SpecularFactor, gSpecularPower);                               
			if (SpecularFactor > 0)
			{
				float Distance = length(Position0.xyz - lights[pointlightID].Position);
				float Attenuation = 1.0f / pow(max(0.0f, 1.0f - (Distance/size)), lights[pointlightID].AmbientIntensity);
				specularAddetive = (vec4(lights[pointlightID].Color, 1.0f) * ( 1 - Normal0.w) * SpecularFactor) / Attenuation;
			}
		
			//pointlight attenuations
			fragment_color *= pow(max(0.0f, 1.0f - (Distance/size)), lights[pointlightID].AmbientIntensity);
		
			Diffuse0 = texture(Diffuse, UV);
			fragment_color = (fragment_color * Diffuse0 + specularAddetive); // * universalInten;
			
			//fragment_color = vec4(1,1,1,1);
		}
		
	}
	//fragment_color += vec4(lights[pointlightID].Color, 1) * 0.1f;
}