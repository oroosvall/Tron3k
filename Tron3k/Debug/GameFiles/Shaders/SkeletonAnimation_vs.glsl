#version 410
layout(location = 0) in vec3 bonePos;
layout(location = 1) in vec2 boneUV;
layout(location = 2) in vec3 boneNormal;
layout(location = 3) in vec3 boneTagent;
layout(location = 4) in vec4 boneIndices;
layout(location = 5) in vec4 skinWeights;

uniform mat4 WorldMatrix;
uniform mat4 ViewProjMatrix;

uniform float delta;

struct animationMatrices
{
	mat4 bMatrix;
};

layout (std140) uniform boneMatrices
{
	animationMatrices inverseBindpose[100];	
};	

layout (std140) uniform boneMatricesAsd
{
	animationMatrices inverseBindposeSecond[100];	
};	

layout(location = 0) out vec3 bonePos0;
layout(location = 1) out vec2 boneUV0;
layout(location = 2) out vec3 boneNormal0;
layout(location = 3) out vec3 boneTagent0;

vec4 matToQuat(mat4 m)
{
	vec4 q = vec4(0.0f);
	
	q.w = sqrt(max(0, 1.0f + m[0][0] + m[1][1] + m[2][2]))/2.0f;
	q.x = sqrt(max(0, 1.0f + m[0][0] - m[1][1] - m[2][2]))/2.0f;
	q.y = sqrt(max(0, 1.0f - m[0][0] + m[1][1] - m[2][2]))/2.0f;
	q.z = sqrt(max(0, 1.0f - m[0][0] - m[1][1] + m[2][2]))/2.0f;
	q.x *= sign(q.x * (m[2][1] - m[1][2]));
	q.y *= sign(q.y * (m[0][2] - m[2][0]));
	q.z *= sign(q.z * (m[1][0] - m[0][1]));
	                     
	return q;
}

vec4 slerp(vec4 q1, vec4 q2, float t)
{
	float d = acos(dot(q1/normalize(q1), q2/normalize(q2)));
	float sd = sin(d);
	return sin((1.0f - t) * d) / sd * q1 + sin(t*d)/sd * q2;
}

mat4 interpolate(mat4 m1, mat4 m2, float t)
{
	//vec4 m1Pos = m1[3];
	//vec4 m2Pos = m2[3];
	//
	//vec4 q1 = matToQuat(m1);
	//vec4 q2 = matToQuat(m2);
	//
	//vec4 p = mix(m1Pos, m2Pos, t);
	//
	//vec4 q = slerp(q1, q2, t);
	//
	////float xx = q[0] * q[0]; float xy = q[0] * q[1]; float xz = q[0] * q[2]; float xw = q[0] * q[3];
	////float yy = q[1] * q[1]; float yz = q[1] * q[2]; float yw = q[1] * q[3];
	////float zz = q[2] * q[2]; float zw = q[2] * q[3];
	////
	////mat3 r = mat3( 1 - 2 * yy - 2 * zz, 2 * xy - 2* zw, 2* xz + 2*yw,
	////				2* xy + 2 * zw, 1 - 2* xx - 2*zz, 2*yz - 2 *xw,
	////				2 * xz - 2*yw, 2 * yz + 2* xw, 1 - 2*xx - 2 * yy);
	////
	////mat4 m = mat4(vec4(r[0],0), vec4(r[1],0), vec4(r[2],0), p);
	////m[3] = p;
	//
	////mat4 m = mat4(m1[0], m1[1], m1[2], p);
	//
	//q = q1;
	//q = normalize(q);
	//	
	//mat4 m = mat4(
	//1.0f - 2.0f*q.y*q.y - 2.0f*q.z*q.z, 2.0f*q.x*q.y - 2.0f*q.z*q.w, 2.0f*q.x*q.z + 2.0f*q.y*q.w, 0.0f,
	//2.0f*q.x*q.y + 2.0f*q.z*q.w, 1.0f - 2.0f*q.x*q.x - 2.0f*q.z*q.z, 2.0f*q.y*q.z - 2.0f*q.x*q.w, 0.0f,
	//2.0f*q.x*q.z - 2.0f*q.y*q.w, 2.0f*q.y*q.z + 2.0f*q.x*q.w, 1.0f - 2.0f*q.x*q.x - 2.0f*q.y*q.y, 0.0f,
	//0.0f, 0.0f, 0.0f, 1.0f);
	//
	////m = transpose(m);
    //
	//
	//m[3] = p;
	
	return m1;
	
}

void main()
{

	mat4 m1 = interpolate(inverseBindpose[int(boneIndices.x)].bMatrix, inverseBindposeSecond[int(boneIndices.x)].bMatrix, delta);
	mat4 m2 = interpolate(inverseBindpose[int(boneIndices.y)].bMatrix, inverseBindposeSecond[int(boneIndices.y)].bMatrix, delta);
	mat4 m3 = interpolate(inverseBindpose[int(boneIndices.z)].bMatrix, inverseBindposeSecond[int(boneIndices.z)].bMatrix, delta);
	mat4 m4 = interpolate(inverseBindpose[int(boneIndices.w)].bMatrix, inverseBindposeSecond[int(boneIndices.w)].bMatrix, delta);

	bonePos0 = 	((vec4(bonePos, 1.0f) * m1) * skinWeights.x).xyz;
	boneNormal0 = ((vec4(boneNormal, 1.0f) * m1) * skinWeights.x).xyz;
	boneTagent0 = ((vec4(boneTagent, 1.0f) * m1) * skinWeights.x).xyz;
	
	bonePos0 += ((vec4(bonePos, 1.0f) * m2) * skinWeights.y).xyz;
	boneNormal0 += ((vec4(boneNormal, 1.0f) * m2) * skinWeights.y).xyz;
	boneTagent0 += ((vec4(boneTagent, 1.0f) * m2) * skinWeights.y).xyz;
	
	bonePos0 += ((vec4(bonePos, 1.0f) * m4) * skinWeights.z).xyz;
	boneNormal0 += ((vec4(boneNormal, 1.0f) * m3) * skinWeights.z).xyz;
	boneTagent0 += ((vec4(boneTagent, 1.0f) * m3) * skinWeights.z).xyz;
	
	bonePos0 += ((vec4(bonePos, 1.0f) * m4) * skinWeights.w).xyz;
	boneNormal0 += ((vec4(boneNormal, 1.0f) * m4) * skinWeights.w).xyz;
	boneTagent0 += ((vec4(boneTagent, 1.0f) * m4) * skinWeights.w).xyz;

	bonePos0 = (vec4(bonePos0, 1.0f) * WorldMatrix).xyz;
	boneNormal0 = (vec4(boneNormal0, 0.0f) * WorldMatrix).xyz;
	boneTagent0 = (vec4(boneTagent0, 0.0f) * WorldMatrix).xyz;
	gl_Position = ViewProjMatrix * vec4(bonePos0.xyz, 1.0f);
	boneUV0 = boneUV;
}   