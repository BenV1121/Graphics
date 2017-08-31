#pragma once

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
void freeShader(Shader &);

struct Framebuffer
{
	unsigned handle;
	unsigned width, height;
};

struct Texture
{
	unsigned handle;
};

Texture makeTexture(unsigned w, unsigned b, unsigned c, const unsigned char * pixels);

void freeTexture(Texture &t);