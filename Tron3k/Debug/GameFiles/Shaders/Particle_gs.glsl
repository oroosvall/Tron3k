#version 410
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout (location = 0) in vec4 dir[1];
layout (location = 1) in vec4 iDir[1];

//layout(location = 0) out vec4 dirOut;
//layout(location = 1) out vec4 iDirOut;
layout(location = 0) out vec2 uv_frag;
layout(location = 1) out float intensity;

layout(location = 2) out vec3 wPos;
layout(location = 3) out vec3 normal;

uniform mat4 VP;
uniform vec3 cam;
uniform vec2 size;
uniform int scaleDir;

void main() 
{	
	if(dir[0].w > 0.0f)
	{
		vec3 pos = gl_in[0].gl_Position.xyz;
		
		vec3 cam_normal = normalize(cam - pos);
		
		vec3 up = dir[0].xyz;
		vec3 right = cross(cam_normal, up);
		
		intensity = iDir[0].w;
		
		vec3 v = pos;
		
		up *= (size.y / 2);


		right *= (size.x / 2);


		if (scaleDir == 1)
		{
			//scale up with time
			up *= 1.0f - iDir[0].w;
			right *= 1.0f - iDir[0].w;		
		}

		else if (scaleDir == 0)
		{
			//dont scale with time
		}
		else if (scaleDir == -1)
		{
			//scale down with time
			up *= iDir[0].w;
			right *= iDir[0].w;
		}


		
		normal = cam_normal;
		
		v = pos;
		v -= right;
		v -= up;
		uv_frag = vec2(0.0f, 0.0f);
		gl_Position = VP * vec4(v, 1.0f);
		wPos = v;
		EmitVertex();
		
		v = pos;
		v += right;
		v -= up;
		uv_frag = vec2(1.0f, 0.0f);
		gl_Position = VP * vec4(v, 1.0f);
		wPos = v;
		EmitVertex();

		v = pos;
		v -= right;
		v += up;
		uv_frag = vec2(0.0f, 1.0f);
		gl_Position = VP * vec4(v, 1.0f);
		wPos = v;
		EmitVertex();		

		v = pos;
		v += right;
		v += up;
		uv_frag = vec2(1.0f, 1.0f);
		gl_Position = VP * vec4(v, 1.0f);
		wPos = v;
		EmitVertex();
		
		EndPrimitive();
	}
	
}