// Graphics\resources\shaders\test.frag

#version 450
out vec4 outColor;
in vec2 vUV;
layout(location = 0) uniform sampler2D map;
void main ()
{
	outColor = texture(map, vUV.xy);
}