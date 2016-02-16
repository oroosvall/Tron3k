#version 410

layout(location = 0) in vec2 uv_frag;

uniform sampler2D tex;

layout (location = 2) out vec4 fragment_color;
void main () 
{
	fragment_color = texture(tex, uv_frag);
	fragment_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}