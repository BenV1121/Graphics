#version 450

layout(location = 0) in vec4 position;

layout(location = 0) uniform mat4 lproj;
layout(location = 1) uniform mat4 lview;
layout(location = 2) uniform mat4 model;

void main()
{
	gl_Position = lproj * lview * model * position;
}

