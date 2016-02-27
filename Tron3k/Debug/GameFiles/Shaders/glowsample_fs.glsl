#version 410

layout (location = 0) in vec2 UV;

uniform sampler2D glowTexture;
  
uniform float pixeluvX;
uniform float pixeluvY;
  
layout (location = 5) out vec4 GlowMap;
							
const float kernel[51] = float[51](0.001776, 0.002268, 0.002869,
0.003592, 0.004453, 0.005465, 0.00664, 0.007988, 0.009515, 0.01122, 0.0131, 0.015142, 0.017328, 0.019634, 0.022024, 0.024461, 0.026896, 0.02928, 0.031559, 0.033676, 0.035579, 0.037215, 0.038539, 0.039514, 0.040111, 0.040312, 0.040111, 0.039514, 0.038539, 0.037215, 0.035579, 0.033676, 0.031559, 0.02928, 0.026896, 0.024461, 0.022024, 0.019634, 0.017328, 0.015142, 0.0131, 0.01122, 0.009515, 0.007988, 0.00664, 0.005465, 0.004453, 0.003592, 0.002869, 0.002268, 0.001776);

void main()
{	
	
	vec4 color = vec4(0.0f); //texture(glowTexture, UV);
    
    //color += texture(glowTexture, UV + vec2(pixeluvX,0));
    //color += texture(glowTexture, UV + vec2(pixeluvX*2,0));
    //color += texture(glowTexture, UV + vec2(pixeluvX*3,0));
    //color += texture(glowTexture, UV + vec2(pixeluvX*4,0));
    //color += texture(glowTexture, UV + vec2(pixeluvX*5,0));
    //
    //color += texture(glowTexture, UV + vec2(-pixeluvX,0));
    //color += texture(glowTexture, UV + vec2(-pixeluvX*2,0));
    //color += texture(glowTexture, UV + vec2(-pixeluvX*3,0));
    //color += texture(glowTexture, UV + vec2(-pixeluvX*4,0));
    //color += texture(glowTexture, UV + vec2(-pixeluvX*5,0));
    
	int i;
    for(i = -25; i < 25; i++)
    {
        color += texture(glowTexture, UV + vec2(pixeluvX*i,0)) * kernel[i+25];
    }
    
    //color /= 11.0f;
    
    color.w = 1.0f;
	
	GlowMap = color;
}