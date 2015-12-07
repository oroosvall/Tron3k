#version 410
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUV;

layout (location = 0) out vec2 UVOut;  

void main()
{
	gl_Position = vec4(VertexPos ,1.0);
	UVOut = VertexUV;
}

//blur[1 + n] = VertexUV + vec2(0.0, -pixeluvY * 6);
//blur[2 + n] = VertexUV + vec2(0.0, -pixeluvY * 5);
//blur[3 + n] = VertexUV + vec2(0.0, -pixeluvY * 4);
//blur[4 + n] = VertexUV + vec2(0.0, -pixeluvY * 3);
//blur[5 + n] = VertexUV + vec2(0.0, -pixeluvY * 2);
//blur[6 + n] = VertexUV + vec2(0.0, -pixeluvY);
//blur[7 + n] = VertexUV + vec2(0.0,  pixeluvY);
//blur[8] = VertexUV + vec2(0.0,  pixeluvY * 2);
//blur[9] = VertexUV + vec2(0.0,  pixeluvY * 3);
//blur[10] = VertexUV + vec2(0.0, pixeluvY * 4);
//blur[11] = VertexUV + vec2(0.0, pixeluvY * 5);
//blur[12] = VertexUV + vec2(0.0, pixeluvY * 6);
//blur[13] = VertexUV + vec2(0.0, pixeluvY * 7);