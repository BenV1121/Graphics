#pragma once

#include "..\include\glm\glm.hpp"

struct Vertex
{
	glm::vec4 position; // starting from 0,  +4 floats = 16 bytes 
	glm::vec4 color;	// starting from 16, +4 floats = 32 bytes

	glm::vec2 texCoord; //UV
	glm::vec4 normal;

	glm::vec4 tangent;
	glm::vec4 bitangent;
};