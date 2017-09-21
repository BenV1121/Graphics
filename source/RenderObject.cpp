#define GLM_FORCE_SWIZZLE
#include "glinc.h"
#include "..\include\graphics\RenderObjects.h"
#include "..\include\graphics\Vertex.h"

#ifdef _DEBUG
#include <iostream>
#endif

glm::vec4 calcTangent(const Vertex &v0, const Vertex &v1, const Vertex &v2)
{
	glm::vec4 p1 = v1.position - v0.position;
	glm::vec4 p2 = v2.position - v0.position;

	glm::vec2 t1 = v1.texCoord - v0.texCoord;
	glm::vec2 t2 = v2.texCoord - v0.texCoord;

	return glm::normalize((p1*t2.y - p2*t1.y) / (t1.x*t2.y - t1.y*t2.x));
}

void solveTangents(Vertex *v, size_t vsize, const unsigned *idxs, size_t isize)
{
	for (int i = 0; i < isize; i += 3)
	{
		glm::vec4 T = calcTangent(v[idxs[i]], v[idxs[i+1]], v[idxs[i+2]]);

		for (int j = 0; j < 3; ++j)
			v[idxs[i + j]].tangent = glm::normalize(T + v[idxs[i+j]].tangent);
	}

	for (int i = 0; i < vsize; ++i)
		v[i].bitangent = glm::vec4(glm::cross(v[i].normal.xyz(), v[i].tangent.xyz()),0);
}

Geometry makeGeometry(const Vertex *verts, size_t vsize,
	const unsigned *idxs, size_t isize)
{
	Geometry retval = { 0,0,0,isize };

	//Declare our openGl objects and acquire handles.

	glGenBuffers(1, &retval.vbo);
	glGenBuffers(1, &retval.ibo);
	glGenVertexArrays(1, &retval.handle);

	//scoping the variables
	glBindVertexArray(retval.handle);
	glBindBuffer(GL_ARRAY_BUFFER, retval.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retval.vbo);



	//Initialize all of our buffers
	glBufferData(GL_ARRAY_BUFFER, vsize*sizeof(Vertex),
					verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		isize * sizeof(unsigned), idxs, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1); // color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);

	glEnableVertexAttribArray(2); // color
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	glEnableVertexAttribArray(3); // color
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)40);

	glEnableVertexAttribArray(4); // color
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)56);

	glEnableVertexAttribArray(5); // color
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)72);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return retval;
}

void freeGeometry(Geometry &g)
{
	glDeleteBuffers(1, &g.vbo);
	glDeleteBuffers(1, &g.ibo);
	glDeleteVertexArrays(1, &g.handle);
	g = { 0,0,0,0 };
}

Shader makeShader(const char *vsource, const char *fsource)
{
	Shader retval = { 0 };

	retval.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsource, 0);
	glShaderSource(fs, 1, &fsource, 0);

	glCompileShader(vs);
#ifdef _DEBUG
	GLint success = GL_FALSE;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(vs, length, 0, log);
		std::cerr << log << std::endl;
		assert(false &&  "Vertex shader failed to compile!");
		delete[] log;
	}
#endif
	glCompileShader(fs);
#ifdef _DEBUG
	success = GL_FALSE;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(fs, length, 0, log);
		std::cerr << log << std::endl;
		assert(false && "Fragment shader failed to compile!");
		delete[] log;
	}
#endif

	glAttachShader(retval.handle, vs);
	glAttachShader(retval.handle, fs);

	glLinkProgram(retval.handle);
#ifdef _DEBUG
	success = GL_FALSE;
	glGetProgramiv(retval.handle, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(retval.handle, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetProgramInfoLog(retval.handle, length, 0, log);
		std::cerr << log << std::endl;
		assert(false && "Program failed to link!");
		delete[] log;
	}
#endif

	glDeleteShader(vs);
	glDeleteShader(fs);

	return retval;
}

Shader loadUpdateShader(const char *vert_path)
{
	return Shader();
}

Shader makeUpdateShader(const char *vert_src)
{
	return Shader();
}


void freeShader(Shader &s)
{
	glDeleteProgram(s.handle);
	s = { 0 };
}

//Texture makeTexture(unsigned w, unsigned h, unsigned c, const unsigned char * pixels)
//{
//	Texture retval{ 0 };
//	
//	unsigned f = 0;
//	switch (c)
//	{
//	case 1: f = GL_RED;  break;
//	case 2: f = GL_RG;   break;
//	case 3: f = GL_RGB;  break;
//	case 4: f = GL_RGBA; break;
//	}
//
//	glGenTextures(1, &retval.handle);
//	glBindTexture(GL_TEXTURE_2D, retval.handle);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f,
//								GL_UNSIGNED_BYTE, pixels);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//	return retval;
//}

Texture makeTexture(unsigned w, unsigned h, unsigned c, const void* pixels, bool isFloat)
{
	Texture retval{ 0 };

	GLenum f = 0, i = 0;

	switch (c)
	{
	case 0: f = GL_DEPTH_COMPONENT; i = GL_DEPTH24_STENCIL8; break;
	case 1: f = GL_RED;  i  = GL_R32F;    break;
	case 2: f = GL_RG;   i  = GL_RG32F;   break;
	case 3: f = GL_RGB;  i  = GL_RGB32F;  break;
	case 4: f = GL_RGBA; i  = GL_RGBA32F; break;
	}

	glGenTextures(1, &retval.handle);
	glBindTexture(GL_TEXTURE_2D, retval.handle);

	i = ((isFloat || c == 0) ? i : f);

	glTexImage2D(GL_TEXTURE_2D, 0, i, w, h, 0, f, 
								(isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return retval;
}

void freeTexture(Texture &t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0 };
}


CubeTexture makeCubeMap(unsigned w, unsigned h, unsigned c, const void **pixels, bool isFloat)
{
	CubeTexture retval{ 0 };

	GLenum f = 0, i = 0;

	switch (c)
	{
	case 0: f = GL_DEPTH_COMPONENT; i = GL_DEPTH24_STENCIL8; break;
	case 1: f = GL_RED;  i = GL_R32F;    break;
	case 2: f = GL_RG;   i = GL_RG32F;   break;
	case 3: f = GL_RGB;  i = GL_RGB32F;  break;
	case 4: f = GL_RGBA; i = GL_RGBA32F; break;
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f, (isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f, (isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f, (isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f, (isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f, (isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, ((isFloat || c == 0) ? i : f), w, h, 0, f, (isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE), pixels[5]);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return retval;
}

Framebuffer makeFramebuffer(unsigned w, unsigned h, unsigned c,
	bool hasDepth, unsigned nTargets, unsigned nFloatTargets)
{
	Framebuffer retval = { 0,w,h,nTargets + nFloatTargets,0,{0} };

	glGenFramebuffers(1, &retval.handle);
	glBindFramebuffer(GL_FRAMEBUFFER, retval.handle);

	if (hasDepth)
	{
		retval.depthTarget = makeTexture(w, h, 0, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
											retval.depthTarget.handle, 0);
	}

	const GLenum attatchments[8] = 
		{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		  GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		  GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7};

	for (int i = 0; i < retval.nTargets && i < 8; ++i)
	{
		retval.targets[i] = makeTexture(w, h, c, 0, i >= nTargets);
		glFramebufferTexture(GL_FRAMEBUFFER, attatchments[i], retval.targets[i].handle, 0);
	}
	glDrawBuffers(retval.nTargets, attatchments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return retval;
}

void freeFramebuffer(Framebuffer &fb)
{

}

ParticleBuffer makeParticleBuffer(const ParticleBuffer *parts, size_t psize)
{
	ParticleBuffer retval = { 0 };

	glGenVertexArrays(1, &retval.handle[0]);
	glBindVertexArray(retval.handle[0]);
	
	GLfloat data[] = { 1,2,3,4,5 };

	glGenBuffers(1, &retval.vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, retval.vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

	glGenBuffers(1, &retval.vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, retval.vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STREAM_DRAW);

	return retval;
}

CubeFrameBuffer makeCubeFrameBuffer(unsigned w, unsigned h, unsigned c,
	bool hasDepth, unsigned nTargets, unsigned nFloatTargets)
{
	CubeFrameBuffer retval = { 0,w,h,nTargets + nFloatTargets,0,{ 0 } };

	glGenFramebuffers(1, &retval.handle);
	glBindFramebuffer(GL_FRAMEBUFFER, retval.handle);

	if (hasDepth)
	{
		retval.depthTarget = makeTexture(w, h, 0, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			retval.depthTarget.handle, 0);
	}

	const GLenum attatchments[8] =
	{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };

	for (int i = 0; i < retval.nTargets && i < 8; ++i)
	{
		retval.targets[i] = makeTexture(w, h, c, 0, i >= nTargets);
		glFramebufferTexture(GL_FRAMEBUFFER, attatchments[i], retval.targets[i].handle, 0);
	}
	glDrawBuffers(retval.nTargets, attatchments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return retval;
}