#version 450

layout(location = 3)uniform sampler2D diffuseMap;
layout(location = 4)uniform sampler2D specularMap;
layout(location = 5)uniform sampler2D normalMap;
layout(location = 6)uniform float gloss;

in vec2 vUV;
in vec3 vPos;
in mat4 vTBN;

layout(location = 0)out vec4 outDiffuse; 
layout(location = 1)out vec4 outSpecular;
layout(location = 2)out vec4 outPosition;
layout(location = 3)out vec4 outNormal;

void main()
{
	outDiffuse = texture(diffuseMap, vUV);
	outSpecular = texture(specularMap, vUV);

	outNormal = (vTBN*(2*texture(normalMap, vUV)-1));
	outNormal.w = gloss;

	outPosition = vec4(vPos,1);
}