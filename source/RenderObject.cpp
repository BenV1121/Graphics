#include "glinc.h"
#include "..\include\graphics\RenderObjects.h"
#include "..\include\graphics\Vertex.h"

#ifdef _DEBUG
#include <iostream>
#endif

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

void freeShader(Shader &s)
{
	glDeleteProgram(s.handle);
	s = { 0 };
}

Texture makeTexture(unsigned w, unsigned h, unsigned c, const unsigned char * pixels)
{
	Texture retval{ 0 };
	
	unsigned f = 0;
	switch (c)
	{
	case 1: f = GL_RED;  break;
	case 2: f = GL_RG;   break;
	case 3: f = GL_RGB;  break;
	case 4: f = GL_RGBA; break;
	}

	glGenTextures(1, &retval.handle);
	glBindTexture(GL_TEXTURE_2D, retval.handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f,
								GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	return retval;
}

void freeTexture(Texture &t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0 };
}