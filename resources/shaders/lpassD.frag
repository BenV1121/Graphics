#version 450

in vec2 vUV;

layout(location = 1) uniform mat4 view;

layout(location = 2) uniform mat4 lightProj;
layout(location = 3) uniform mat4 lightView;

layout(location = 4) uniform vec4 lightColor;
layout(location = 5) uniform float intensity;

layout(location = 6) uniform sampler2D normalMap;
layout(location = 7) uniform sampler2D positionMap;
layout(location = 8) uniform sampler2D shadowMap;

layout(location = 0) out vec4 outDiffuse;

uniform float shadowBias = 0.01;

uniform mat4 clipToUV = mat4(0.5f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.5f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.5f, 0.0f,
							 0.5f, 0.5f, 0.5f, 1.0f);

void main()
{
	vec4 vPos = texture(positionMap, vUV);
	vec4 sUV = clipToUV * lightProj * lightView * inverse(view) * vPos;

	float visibility = 1;
	if(texture(shadowMap, sUV.xy).r < sUV.z - shadowBias)
		visibility = 0;

	vec3 L = (view * inverse(lightView)[2]).xyz;
	vec3 N = texture(normalMap, vUV).xyz;
	float lamb = max(0,dot(-L, N));

	outDiffuse = lightColor * intensity * lamb * visibility;
}