#version 450

layout(location = 3) uniform mat4 lproj;
layout(location = 4) uniform mat4 lview;
layout(location = 5) uniform sampler2D shadowmap;

in vec2 vUV;
in vec4 vPos;

uniform float shadowBias = 0.01;

uniform mat4 clipToUV = mat4(0.5f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.5f, 0.5f, 0.0f,
							 0.0f, 0.0f, 0.5f, 0.0f,
							 0.5f, 0.5f, 0.5f, 1.0f);

out vec4 outColor;

void main()
{
	vec4 sUV = clipToUV * lproj * lview * vPos;

	float visibility = 1;
	if(texture(shadowmap,sUV.xy).r < sUV.z - shadowBias)
	{
	visibility - 0;
	}

	outColor = vec4(1,1,0,1) * visibility;
}

