#version 450

out vec4 outColor;

in vec2 vUV;
in vec3 vPos;
in vec3 vNormal;

layout (location = 3) uniform sampler2D map;

layout (location = 4) uniform vec3 LP;
layout (location = 5) uniform vec3 lightDirection;
layout (location = 6) uniform vec4 lightColor;

void main()
{
	vec4 surfaceColor = texture(map, vUV);
	vec3 DISP = vPos - LP;

	float d = length(DISP);
	float attenuation = 1.0/(d*d);
	vec3 L = normalize(DISP);
	vec3 N = vNormal;

	float lambertianFactor = max(0,dot(N, L));

	vec4 lambertianTerm = surfaceColor * lambertianFactor * lightColor;
	
    outColor = lambertianTerm;

	outColor = vec4(1,1,0,1);
}