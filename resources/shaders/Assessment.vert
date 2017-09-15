#version 450

// vertex attributes
layout(location = 0)in vec4 position;
layout(location = 2)in vec2 texcoord;
layout(location = 3)in vec4 normal;
layout(location = 4)in vec4 tangent;
layout(location = 5)in vec4 bitangent;

// Camera
layout(location = 0)uniform mat4 proj;
layout(location = 1)uniform mat4 view;

// Model
layout(location = 2)uniform mat4 model;

// Particle
//layout(location = 0) in vec4 vertex;

// output to frag shader
out vec2 vUV;
out vec3 vPos;
out mat4 vTBN;
out vec3 vNormal;
out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;

void main()
{
	vTBN = model * mat4(tangent, bitangent, normal, vec4(0,0,0,0));
	vUV  = texcoord;
	vPos = (model * position).xyz;
	vNormal = (model * normal).xyz;

	float scale = 10.0f;
	//TexCoords = vertex.zw;
	//ParticleColor = color;

	gl_Position = proj * view * model * position;

}