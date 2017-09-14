#version 450
// final output
out vec4 outColor;

// input from frag shader
in vec2 vUV;
in vec3 vPos;
in mat4 vTBN;
in vec3 vNormal;
in vec4 ParticleColor;

uniform sampler2D sprite;
// Camera
layout(location = 1) uniform mat4 view;

// Surface Material Data
layout(location = 3) uniform sampler2D diffuseMap;
layout(location = 6) uniform float     gloss;

// Light Data
layout(location = 8) uniform mat4 l_view;
layout(location = 9) uniform vec4  l_color;
layout(location = 10) uniform float l_intensity;

// Output Variables // 4 targets, 3 color and 1 float
layout(location = 0) out vec4 outFinal;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outSpecular;
layout(location = 3) out vec4 outNormal;

// illumination model factors
	// surface normal and light direction
float calc_lambert(in vec3 N, in vec3 L);
	// surface normal, light direction, direction to eyeballs, and specular power
float calc_phong(in vec3 N, in vec3 L, in vec3 V, in float specPower);

void main()
{
	// Read surface data
	vec3 normal   = vNormal; //(vTBN*(2*texture(normalMap, vUV)-1)).xyz;	
	vec4 diffuse  = texture(diffuseMap, vUV);
	//vec4 specular = texture(specularMap, vUV);
	
	// calculate light direction
	vec3 lDir = -l_view[2].xyz;


	// calculate our lighting factors
	float lamb = calc_lambert(normal, lDir);


	outFinal = diffuse  * lamb * l_color * l_intensity;
}


float calc_lambert(in vec3 N, in vec3 L)
{
	return max(0, dot(N,-L));
}

float calc_phong(in vec3 N, in vec3 L, in vec3 V, in float specPower)
{
	if(dot(N,-L) <= 0) return 0;
	vec3 R = reflect(L, N);
	return pow(max(0, dot(V, -R)), specPower); // exponential falloff
}