#version 410

#define FXAA_EDGE_THRESHOLD_MIN 1.0f/8.0f
#define FXAA_EDGE_THRESHOLD 1.0f/16.0f
#define FXAA_SUBPIX 1
#define FXAA_SUBPIX_TRIM_SCALE 1
#define FXAA_SUBPIX_TRIM 1.0f/4.0f
#define FXAA_SUBPIX_CAP 3.0f/4.0f


layout (location = 0) in vec2 UV;

uniform int Use;
	
uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D GlowMap;	
uniform sampler2D GlowMap2;
uniform sampler2D Depth;

vec4 Position0;
vec4 Diffuse0;
vec4 Normal0;
vec4 Depth0;
vec4 glowValue;

uniform float universalInten;

uniform float pixeluvX;
uniform float pixeluvY;

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

uniform vec3 eyepos;

float gSpecularPower = 200.0f;
float gMatSpecularIntensity = 0.4f;
	
vec3 ambientLightPosition = vec3(131.0f, 45.0f, 0.0f);
vec3 ambientLightDirection = vec3(-0.16f, -0.36f, 0.61f);
vec3 ambientLightColor = vec3(0.7f, 0.7f , 1.0f);
float ambientLightDiffuseIntensity = 0.1f;
float ambientLightAmbientIntensity = 0.1f;

out vec4 fragment_color;                                                                                       

const float kernel[41] = float[41](0.003848, 0.004894, 0.006148,
0.007629, 0.009351, 0.011321, 0.013537, 0.01599, 0.018655,
0.021497, 0.02447, 0.027511, 0.030552, 0.033513, 0.03631,
0.038858, 0.041076, 0.042888, 0.04423, 0.045056, 0.045335,
0.045056, 0.04423, 0.042888, 0.041076, 0.038858, 0.03631,
0.033513, 0.030552, 0.027511, 0.02447, 0.021497, 0.018655,
0.01599, 0.013537, 0.011321, 0.009351, 0.007629, 0.006148, 0.004894, 0.003848);

//const float kernel[101] = float[101](0.000664, 0.000761, 0.000871, 0.000993, 0.00113, 0.001282, 0.00145, 0.001635, 0.00184, 0.002064, 0.002309, 0.002575, 0.002865, 0.003179, 0.003517, 0.00388, 0.004269, 0.004684, 0.005125, 0.005593, 0.006085, 0.006604, 0.007146, 0.007711, 0.008299, 0.008906, 0.009531, 0.010172, 0.010826, 0.01149, 0.012161, 0.012836, 0.013511, 0.014182, 0.014845, 0.015496, 0.016131, 0.016746, 0.017336, 0.017896, 0.018425, 0.018916, 0.019366, 0.019773, 0.020132, 0.020441, 0.020697, 0.020899, 0.021044, 0.021132, 0.021161, 0.021132, 0.021044, 0.020899, 0.020697, 0.020441, 0.020132, 0.019773, 0.019366, 0.018916, 0.018425, 0.017896, 0.017336, 0.016746, 0.016131, 0.015496, 0.014845, 0.014182, 0.013511, 0.012836, 0.012161, 0.01149, 0.010826, 0.010172, 0.009531, 0.008906, 0.008299, 0.007711, 0.007146, 0.006604, 0.006085, 0.005593, 0.005125, 0.004684, 0.004269, 0.00388, 0.003517, 0.003179, 0.002865, 0.002575, 0.002309, 0.002064, 0.00184, 0.001635, 0.00145, 0.001282, 0.00113, 0.000993, 0.000871, 0.000761, 0.000664
//);

float rgbToLuma(vec3 color)
{
    return color.r*0.2126f + color.g*0.7152 + color.b*0.0722;
}

vec4 fxaaPass(sampler2D tex)
{
    vec3 rgbN = texture(tex, vec2(UV.x, UV.y) + vec2(0,pixeluvY)).rgb;
    vec3 rgbW = texture(tex, vec2(UV.x, UV.y) + vec2(-pixeluvX,0)).rgb;
    vec3 rgbM = texture(tex, vec2(UV.x, UV.y)).rgb;
    vec3 rgbE = texture(tex, vec2(UV.x, UV.y) + vec2(pixeluvX,0)).rgb;
    vec3 rgbS = texture(tex, vec2(UV.x, UV.y) + vec2(0,-pixeluvY)).rgb;
    
    float lumaN  = rgbToLuma(rgbN);
    float lumaW  = rgbToLuma(rgbW);
    float lumaM  = rgbToLuma(rgbM);
    float lumaE  = rgbToLuma(rgbE);
    float lumaS  = rgbToLuma(rgbS);
    
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    float range = rangeMax - rangeMin;
    if(range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD))
    {
        return vec4(rgbM, 1.0f);
    }
    
    
    float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
    float rangeL = abs(lumaL - lumaM);
    float blendL = max(0.0, (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE; 
    blendL = min(FXAA_SUBPIX_CAP, blendL);
    
    vec3 rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;
    // ...
    vec3 rgbNW = texture(tex, UV + vec2(-pixeluvX, -pixeluvY)).xyz;
    vec3 rgbNE = texture(tex, UV + vec2( pixeluvX, -pixeluvY)).xyz;
    vec3 rgbSW = texture(tex, UV + vec2(-pixeluvX, pixeluvY)).xyz;
    vec3 rgbSE = texture(tex, UV + vec2( pixeluvX, pixeluvY)).xyz;
    rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
    rgbL *= vec3(1.0/9.0);
    
    return vec4(rgbL, 1.0f);
    
}

void main()
{
	fragment_color = vec4(0);
	
	Position0 = texture(Position, vec2(UV.x, UV.y));
	//Diffuse0 = texture(Diffuse, vec2(UV.x, UV.y));
    //Diffuse0 = fxaaPass();
	//glowValue = texture(GlowMap, vec2(UV.x, UV.y));
	
	vec4 specularAddetive = vec4(0,0,0,0);
	
	float len = length(Position0.xyz - eyepos);
	if(len < 500)
	{
		Diffuse0 = fxaaPass(Diffuse);
        
		Normal0 = texture(Normal, vec2(UV.x, UV.y));
		
		vec3 normal3 = vec3(Normal0);	
		float DiffuseFactor = dot(normal3, -ambientLightDirection);                                                                                                           
																				   
		if (DiffuseFactor > 0) 
		{    
		
			fragment_color = vec4(0);
			fragment_color = vec4(ambientLightColor, 1.0f) * (ambientLightDiffuseIntensity * 3) * DiffuseFactor;                                                                                          
			vec3 VertexToEye = normalize(eyepos - Position0.xyz);                             
			vec3 LightReflect = normalize(reflect(ambientLightDirection, normal3));                     
			float SpecularFactor = dot(VertexToEye, LightReflect); 		
			SpecularFactor = pow(SpecularFactor, gSpecularPower);
			
			if (SpecularFactor > 0)				
				specularAddetive += (vec4(ambientLightColor, 1.0f) * ( 1 - Normal0.w) * SpecularFactor);
		}       
		
		vec4 ambientForce = vec4(ambientLightColor, 1) * ambientLightAmbientIntensity;
		fragment_color = fragment_color * Diffuse0 +  Diffuse0 * ambientForce;
	}
	else //dont color the skybox!
	{
		fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
	}
	vec4 sum = vec4(0);
	
	//top left quadrant
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 1 , -pixeluvY * 1 )) * 0.058488;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 1 , -pixeluvY * 2 )) * 0.014662;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 1 , -pixeluvY * 3 )) * 0.001446;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 2 , -pixeluvY * 1 )) * 0.014662;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 2 , -pixeluvY * 2 )) * 0.003676;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 2 , -pixeluvY * 3 )) * 0.000363;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 3 , -pixeluvY * 1 )) * 0.001446;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 3 , -pixeluvY * 2 )) * 0.000363;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 3 , -pixeluvY * 3 )) * 0.000036;
    //
	////top right quadrant   
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 1 , -pixeluvY * 1 )) * 0.058488;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 1 , -pixeluvY * 2 )) * 0.014662;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 1 , -pixeluvY * 3 )) * 0.001446;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 2 , -pixeluvY * 1 )) * 0.014662;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 2 , -pixeluvY * 2 )) * 0.003676;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 2 , -pixeluvY * 3 )) * 0.000363;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 3 , -pixeluvY * 1 )) * 0.001446;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 3 , -pixeluvY * 2 )) * 0.000363;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 3 , -pixeluvY * 3 )) * 0.000036;
    //
	////bot left quadrant  
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 1 , pixeluvY * 1 )) * 0.058488;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 1 , pixeluvY * 2 )) * 0.014662;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 1 , pixeluvY * 3 )) * 0.001446;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 2 , pixeluvY * 1 )) * 0.014662;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 2 , pixeluvY * 2 )) * 0.003676;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 2 , pixeluvY * 3 )) * 0.000363;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 3 , pixeluvY * 1 )) * 0.001446;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 3 , pixeluvY * 2 )) * 0.000363;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 3 , pixeluvY * 3 )) * 0.000036;
    //
	////bot left quadrant  
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 1 , pixeluvY * 1 )) * 0.058488;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 1 , pixeluvY * 2 )) * 0.014662;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 1 , pixeluvY * 3 )) * 0.001446;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 2 , pixeluvY * 1 )) * 0.014662;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 2 , pixeluvY * 2 )) * 0.003676;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 2 , pixeluvY * 3 )) * 0.000363;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 3 , pixeluvY * 1 )) * 0.001446;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 3 , pixeluvY * 2 )) * 0.000363;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 3 , pixeluvY * 3 )) * 0.000036;
    //
	////Cross samples
	////up
	//sum += texture(GlowMap, UV + vec2( 0, -pixeluvY * 1)) * 0.092651;
	//sum += texture(GlowMap, UV + vec2( 0, -pixeluvY * 2)) * 0.023226;
	//sum += texture(GlowMap, UV + vec2( 0, -pixeluvY * 3)) * 0.002291;
    //
	////left
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 1, 0)) * 0.092651;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 2, 0)) * 0.023226;
	//sum += texture(GlowMap, UV + vec2( -pixeluvX * 3, 0)) * 0.002291;
    //
	////right
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 1, 0)) * 0.092651;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 2, 0)) * 0.023226;
	//sum += texture(GlowMap, UV + vec2( pixeluvX * 3, 0)) * 0.002291;
    //
	////down
	//sum += texture(GlowMap, UV + vec2( 0, pixeluvY * 1)) * 0.092651;
	//sum += texture(GlowMap, UV + vec2( 0, pixeluvY * 2)) * 0.023226;
	//sum += texture(GlowMap, UV + vec2( 0, pixeluvY * 3)) * 0.002291;
    //
	////middle sample
	//sum += texture(GlowMap, UV) * 0.146768;
	//sum = texture(GlowMap2, UV);
	
	int i;
	for(i = -20; i < 21; i++ )
	{
		sum += texture(GlowMap2, UV  + vec2(0.0f, pixeluvY * i))* kernel[i + 20];
	}
	
	//sum += texture(GlowMap, UV) * 0.65f;
    sum += fxaaPass(GlowMap) * 0.65f;
	//sum *= universalInten;
	
	fragment_color += sum + specularAddetive;
}