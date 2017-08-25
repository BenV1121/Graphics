#version 450

in vec4 vCol;

in vec2 vUV;
in vec4 vNormal;

out vec4 outColor;

layout(location = 1) uniform sampler2D map;

void main()
{
	outColor = dot(vNormal, -normalize(vec4(1,1,1,0))) * texture(map, vUV);
}