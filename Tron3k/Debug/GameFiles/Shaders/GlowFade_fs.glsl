#version 410

layout (location = 0) in vec2 UV;

uniform float deltaTime;
uniform float falloff;
  
uniform sampler2D glowAdd;
uniform sampler2D self;
  
layout (location = 1) out vec4 GlowMap;

void main()									
{	
	vec4 glow = texture(self, UV);
	vec4 addColor = texture(glowAdd, UV);
	vec4 final;
	
    final = vec4(glow.rgb * glow.w, glow.w);
    
    final -= deltaTime;
    final.rgb += addColor.rgb;
    final.w += addColor.w;
    
    final = clamp(final, vec4(0.0f),vec4(1.0f));
    
    //final = addColor;
    
	//float addDelta = glow.w * deltaTime;
	//glow -= vec4(10.0f * addDelta);
	//final.w = glow.w + addColor.w;
	//final.rgb = (glow.rgb * final.w) + (addColor.rgb * (1.0f - final.w)* addColor.w);
	
	GlowMap = final;
		
}