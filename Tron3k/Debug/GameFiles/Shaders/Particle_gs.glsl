#version 410
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout (location = 0) in vec4 dir[1];
layout (location = 1) in vec4 iDir[1];

//layout(location = 0) out vec4 dirOut;
//layout(location = 1) out vec4 iDirOut;
layout(location = 0) out vec2 uv_frag;

uniform mat4 MVP;
uniform vec3 cam;
uniform vec2 size;

void main() 
{
	//if(dir[0].w > 0.0f)
	//{
	//	vec3 pos = gl_in[0].gl_Position.xyz;
	//	vec3 cam_normal = normalize(cam - pos);
	//	//vec3 up = vec3(0.0f, 1.0f, 0.0f);
	//	vec3 up = dir[0].xyz;
	//	vec3 right = cross(cam_normal, up);
    //
	//	up = up * size.y*10;
	//	right = right * size.x*10;
    //
	//	vec3 cPos = pos;
    //
	//	//VTX 1
	//	cPos = pos - right - up;
	//	gl_Position = MVP * vec4(cPos, 1.0f);
	//	uv_frag = vec2(0.0f, 0.0f);
	//	EmitVertex();
    //
	//	//VTX 2
	//	cPos = pos + right - up;
	//	gl_Position = MVP * vec4(cPos, 1.0f);
	//	uv_frag = vec2(1.0f, 0.0f);
	//	EmitVertex();
    //
	//	//VTX 3
	//	cPos = pos + up - right;
	//	gl_Position = MVP * vec4(cPos, 1.0f);
	//	uv_frag = vec2(0.0f, 1.0f);
	//	EmitVertex();
    //
	//	//VTX 4
	//	cPos = pos + up + right;
	//	gl_Position = MVP * vec4(cPos, 1.0f);
	//	uv_frag = vec2(1.0f, 1.0f);
	//	EmitVertex();
	//	
	//	EndPrimitive();
	//}
	
	//if(dir[0].w > 0.0f)
	//{
//first node

		vec3 pos1 = vec3(0,5,0);

		vec3 pos2 = vec3(100, 5, 100);
		//pos2.x += 20;
		
		gl_Position = vec4(pos1, 1);
		gl_Position.y += 1.3;
		gl_Position = MVP * gl_Position;
		//UV = uv1 + vec2(0,1);
		EmitVertex();
		
		//second node
		gl_Position = vec4(pos1, 1);
		gl_Position.y -= 1.5;
		gl_Position = MVP * gl_Position;
		//UV = uv1;
		EmitVertex();

		//third node
		gl_Position = vec4(pos2, 1);
		gl_Position.y += 1.3;
		gl_Position = MVP * gl_Position;
		//UV = uv2 + vec2(0,1);
		EmitVertex();
		
		//third node
		gl_Position = vec4(pos2, 1);
		gl_Position.y -= 1.5;
		gl_Position = MVP * gl_Position;
		//UV = uv2;
		EmitVertex();
		
		EndPrimitive();
	// result makes no sense
	//}
}