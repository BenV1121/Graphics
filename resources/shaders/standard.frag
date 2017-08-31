#version 450

in vec2 vUV;

layout(location = 1)uniform mat4 view;
layout(location = 3)uniform sampler2D normalMap;
layout(location = 4)uniform sampler2D diffuseMap;
layout(location = 5)uniform vec3 L;

out vec4 outColor;
in mat4 vTBN;

float lambert(vec3 N, vec3 L)
{
	return max(0,dot(N,-L));
}

float phong(vec3 N, vec3 L, vec3 V, float power)
{
	vec3 R = reflect(-L, N);
	float base = max(0,dot(V,R));
	return pow(base, power);
}

void main()
{
	vec4 tN = 1-2*texture(normalMap, vUV);
	tN.a = 0;
	tN = normalize(tN);

	vec3 N = (vTBN * tN).xyz;
	vec3 V = view[2].xyz;
	
	float spec = phong(N,L,V,1.0);
	float lamb = lambert(N,L);

	outColor = texture(diffuseMap,vUV) * lamb + vec4(1,1,1,1) * spec;
}