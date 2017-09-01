#version 450

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec4 normal;
layout(location = 4) in vec4 tangent;
layout(location = 5) in vec4 bitangent;

layout(location = 0)uniform mat4 proj;
layout(location = 1)uniform mat4 view;

layout(location = 2)uniform mat4 model;

out vec2 vUV;
out mat4 vTBN;
out vec3 vPos;

out vec4 TEST;

void main()
{
	gl_Position = proj * view * model * position;
	
	vUV = texcoord;
	vTBN = model * mat4(tangent, bitangent, normal, vec4(0,0,0,0));
	vPos = (model * position).xyz;

	//TEST - vec4(texcoord,0,1);
}