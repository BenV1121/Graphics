#pragma once

#include "glm\ext.hpp"
#include "..\include\graphics\Vertex.h"

#include <iostream>
#include <vector>

//declare our vertex
struct Vertex;

struct Geometry
{
	unsigned handle, // Vertex Array Object 
			 vbo,	 // Vertex Bufferr
			 ibo,	 // Index Buffer
			 size;	 // How many indices make up this model
};

Geometry makeGeometry(const Vertex *verts, size_t vsize,
					  const unsigned *idxs, size_t isize);

void freeGeometry(Geometry &g);

void solveTangents(Vertex *v, size_t vsize,
					const unsigned *idxs, size_t isize);

struct Shader
{
	unsigned handle;
};

Shader makeShader(const char *vsource, const char *fsource);
Shader loadUpdateShader(const char *vert_path);
Shader makeUpdateShader(const char *vert_src);
void freeShader(Shader &);


struct Texture
{
	unsigned handle;
};


Texture makeTexture(unsigned w, unsigned h, unsigned c, const void *pixels, bool isFloat = false);

void freeTexture(Texture &t);

struct CubeTexture
{
	unsigned handle;
};

CubeTexture makeCubeMap(unsigned w, unsigned h, unsigned c,
						const void **pixels, bool isFloat = false);

struct Framebuffer
{
	unsigned handle, width, height, nTargets;
	Texture depthTarget;
	Texture targets[8];
};

Framebuffer makeFramebuffer(unsigned w, unsigned h, unsigned c,
	bool hasDepth, unsigned nTargets, unsigned nFloatTargets);

struct CubeFrameBuffer
{
	unsigned handle, width, height, nTargets;
	Texture depthTarget;
	Texture targets[8];
};

CubeFrameBuffer makeCubeFrameBuffer(unsigned w, unsigned h, unsigned c,
	bool hasDepth, unsigned nTargets, unsigned nFloatTargets);

struct Particle
{
	glm::vec2 position, velocity;
	glm::vec4 color;
	float life;

	Particle()
		: position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};

struct ParticleVertex
{
	glm::vec4 position;
	glm::vec3 velocity;
	glm::vec4 color;
};


struct ParticleBuffer { unsigned handle[2], vbo[2], size; };

ParticleBuffer makeParticleBuffer(const ParticleBuffer *parts, size_t psize);



//unsigned int nr_particles = 500;
//std::vector<Particle> particles;
//
//for (GLuint i = 0; i < nr_particles: ++i)
//	particles.push)