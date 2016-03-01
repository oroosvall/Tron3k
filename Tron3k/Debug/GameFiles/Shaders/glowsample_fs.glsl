#version 410

layout (location = 0) in vec2 UV;

uniform sampler2D glowTexture;

uniform float pixeluvX;
  
layout (location = 5) out vec4 GlowMap;

const float kernel[41] = float[41](0.003848, 0.004894, 0.006148,
0.007629, 0.009351, 0.011321, 0.013537, 0.01599, 0.018655,
0.021497, 0.02447, 0.027511, 0.030552, 0.033513, 0.03631,
0.038858, 0.041076, 0.042888, 0.04423, 0.045056, 0.045335,
0.045056, 0.04423, 0.042888, 0.041076, 0.038858, 0.03631,
0.033513, 0.030552, 0.027511, 0.02447, 0.021497, 0.018655,
0.01599, 0.013537, 0.011321, 0.009351, 0.007629, 0.006148, 0.004894, 0.003848);

void main()									
{	
	//vec4 color = texture(glowTexture, UV);
	vec4 color = vec4(0.0f);
	
	int i;
	for(i = -20; i < 21; i++ )
	{
		color += texture(glowTexture, UV  + vec2(pixeluvX * i, 0.0f))* kernel[i + 20];
	}
	color.w = 1.0f;
	GlowMap = color;
		
}