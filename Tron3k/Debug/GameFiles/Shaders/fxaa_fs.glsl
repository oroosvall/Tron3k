#version 410

#define FXAA_EDGE_THRESHOLD_MIN 1.0f/16.0f
#define FXAA_EDGE_THRESHOLD 1.0f/8.0f
#define FXAA_SUBPIX 2
#define FXAA_SUBPIX_TRIM_SCALE 1.0f
#define FXAA_SUBPIX_TRIM 1.0f/4.0f
#define FXAA_SUBPIX_CAP 3.0f/4.0f

layout (location = 0) in vec2 UV;

uniform sampler2D diffuse;

uniform float pixeluvX;
uniform float pixeluvY;

uniform int fxaatoggle;

//layout (location = 1)out vec4 outColor;

out vec4 fragment_color;   

float rgbToLuma(vec3 color)
{
    return color.r*0.2126f + color.g*0.7152 + color.b*0.0722;
}

vec4 fxaaPass(sampler2D tex)
{
    vec3 rgbN = texture(tex, vec2(UV.x, UV.y) + vec2(0.0f,-pixeluvY)).rgb;
    vec3 rgbW = texture(tex, vec2(UV.x, UV.y) + vec2(-pixeluvX,0.0f)).rgb;
    vec3 rgbM = texture(tex, vec2(UV.x, UV.y)).rgb;
    vec3 rgbE = texture(tex, vec2(UV.x, UV.y) + vec2(pixeluvX,0.0f)).rgb;
    vec3 rgbS = texture(tex, vec2(UV.x, UV.y) + vec2(0.0f,pixeluvY)).rgb;
    
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
    
    
    float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25f;
    float rangeL = abs(lumaL - lumaM);
    float blendL = max(0.0f, (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE; 
    blendL = min(FXAA_SUBPIX_CAP, blendL);
    
    vec3 rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;
    // ...
    vec3 rgbNW = texture(tex, UV + vec2(-pixeluvX, -pixeluvY)).xyz;
    vec3 rgbNE = texture(tex, UV + vec2( pixeluvX, -pixeluvY)).xyz;
    vec3 rgbSW = texture(tex, UV + vec2(-pixeluvX, pixeluvY)).xyz;
    vec3 rgbSE = texture(tex, UV + vec2( pixeluvX, pixeluvY)).xyz;
    rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
    rgbL *= vec3(1.0f/9.0f);
    
    return vec4(rgbL, 1.0f);
    
}

void main()
{
	if(fxaatoggle == 1)
		fragment_color = fxaaPass(diffuse);
	else
		fragment_color = texture(diffuse, vec2(UV.x, UV.y));
	//fragment_color = vec4(1.0f);
}
