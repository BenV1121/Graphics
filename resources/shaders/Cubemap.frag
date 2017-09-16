#version 450

in vec3 textureDir;
uniform samplerCube cubemap;

void main()
{
	FragColor = texture(cubemap, textureDir);
}