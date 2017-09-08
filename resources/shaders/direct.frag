#version 450

layout(location = 3) uniform mat4 lproj;
layout(location = 4) uniform mat4 lview;
layout(location = 5) uniform sampler2D shadowmap;

layout(location = 6) uniform mat4 lproj1;
layout(location = 7) uniform mat4 lview1;
layout(location = 8) uniform sampler2D shadowmap1;

in vec2 vUV;
in vec4 vPos;

uniform float shadowBias = 0.01;

uniform mat4 clipToUV = mat4(0.5f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.5f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.5f, 0.0f,
							 0.5f, 0.5f, 0.5f, 1.0f);

out vec4 outColor;

float pcf(in sampler2D shadowMap, in vec4 shadowPosition, int iterations);


void main()
{
	vec4 sUV = clipToUV * lproj * lview * vPos;
	float visibility = 1;
	visibility = pcf(shadowmap, sUV, 16);
	

	vec4 sUV1 = clipToUV * lproj1 * lview1 * vPos;
	float visibility1 = 1;
	visibility1 = pcf(shadowmap1, sUV1, 16);


	outColor = vec4(1,0,0,1) * visibility + vec4(1,1,0,1) * visibility1;
}

float pcf(in sampler2D shadowMap, in vec4 shadowPosition, int iterations)
{
	vec2 sDim = textureSize(shadowMap, 0).xy;
	float retval = 0;
	
	vec2 uv = shadowPosition.xy;
	float z = shadowPosition.z - 0.01;

	for(int i = -iterations; i <= iterations; ++i)
	{
		if(!(texture(shadowMap, uv + vec2(i,0)/sDim).r < z ))
			retval++;

		if(!(texture(shadowMap, uv + vec2(0,i)/sDim).r < z ))
			retval++;
	}
	
	return retval / (iterations*4);
}
