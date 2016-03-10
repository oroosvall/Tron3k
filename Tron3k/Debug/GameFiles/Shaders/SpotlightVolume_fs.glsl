#version 410

layout (location = 0) in float interpolDist;

layout(location = 1)out vec4 fragment_color;

uniform int spotlightID;
uniform vec3 eyepos;

uniform float universalInten;
uniform vec2 gScreenSize;

uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D GlowMap;

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
	
	vec2 UV = gl_FragCoord.xy / gScreenSize;
	
	Position0 = texture(Position, UV).xyz;
	vec3 LightDirection = Position0.xyz - lights[spotlightID].Position;
	float Distance = length(LightDirection);
	
	float spotlength = lights[spotlightID].attenuation.z;
	float cosCone = cos(lights[spotlightID].Cutoff);
	
	if(Distance < spotlength)
	{
		Normal0 = texture(Normal, UV);
		LightDirection = normalize(LightDirection);
		
		float DiffuseFactor = dot(Normal0.xyz, -LightDirection); 
		
		if (DiffuseFactor > 0) 
		{	
			//directional light
			fragment_color = vec4(lights[spotlightID].Color, 1.0f) * lights[spotlightID].DiffuseIntensity * DiffuseFactor;

			//spotlight              
			vec4 specularAddetive = vec4(0);        
			float SpotFactor = dot(LightDirection, lights[spotlightID].Direction);                                      
			
			if (SpotFactor > cosCone)
			{                           
				//specular
				vec3 VertexToEye = normalize(eyepos - Position0);                             
				vec3 LightReflect = normalize(reflect(LightDirection, Normal0.xyz));                     
				float SpecularFactor = dot(VertexToEye, LightReflect);                              
				SpecularFactor = pow(SpecularFactor, gSpecularPower); 

				if (SpecularFactor > 0)
					{
						float Distance = length(Position0.xyz - lights[spotlightID].Position);
						float Attenuation = 1.0f / pow(max(0.0f, 1.0f - (Distance/lights[spotlightID].attenuation.z)), 1);
						specularAddetive = (vec4(lights[spotlightID].Color, 1.0f) * ( 1 - Normal0.w) * SpecularFactor) / Attenuation;
					}
						
				fragment_color *= (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - cosCone)); 	
			}		
			else                                                                                
				fragment_color = vec4(0);

			// Final
			Diffuse0 = texture(Diffuse, UV);
			fragment_color = (fragment_color * Diffuse0 + specularAddetive); // * universalInten;
		}
	}

	// Light volume effect
	vec3 lightToEye = normalize(eyepos - lights[spotlightID].Position);
	float lightToSurfaceAngle = dot(lightToEye, lights[spotlightID].Direction);

	if(lightToSurfaceAngle > cosCone)
		fragment_color += vec4(lights[spotlightID].Color, 1) * pow(interpolDist, 5) * pow(lightToSurfaceAngle, 10); // * universalInten;

	//fragment_color += vec4(lights[spotlightID].Color, 1) * 0.1f;
}