#include "graphics\draw.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Context.h"
#include "glm\ext.hpp"

int main()
{
	Context context;
	context.init(800, 800);

	Vertex verts[4] = { { { .5,  .5,  0, 1 },{ 1,1,0,1 } },   //0
					  { { -.5,  .5,  0, 1 },{ 0,1,1,1 } },   //1
					  { { -.5, -.5,  0, 1 },{ 1,0,1,1 } },   //2
					  { { .5, -.5,  0, 1 },{ 1,0,0,1 } } }; //3

	unsigned idxs[3*2] = { 1,2,3,
						 0,3,1 };

	Geometry g = makeGeometry(verts, 4, idxs, 6);

	unsigned char pixels[] = { 255, 0, 128, 255,
							   255, 128, 255, 0,
							   0, 255, 128, 255,
							   255, 128, 255, 0};

	Texture t_mask = makeTexture(4, 4, 1, pixels);
	Texture t_magyel = makeTexture(4, 1, 3, pixels);

	Framebuffer f = { 0, 800, 800 };

	const char* vsource = // once per vertex
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 3) uniform mat4 mod;\n"
		"void main ()\n"
		"{\n"
		"gl_Position = mod * position;\n"
		"}\n";

	const char* fsource = // once per pixel
		"#version 450\n"
		"out vec4 outColor;\n"

		"layout(location = 0) uniform int k0;\n"
		"layout(location = 1) uniform int k1;\n"
		"layout(location = 2) uniform int k2;\n"

		"void main ()\n"
		"{\n"
		"outColor = vec4(k0,k1,k2,1);\n"
		"}\n";

	Shader s = makeShader(vsource, fsource);

	// RGB texture (3 channels), 2x1

	float xpos = 0, ypos = 0;
	glm::vec2 pos;
	float prevTime = 0;
	float speed = 1.2f;
	while (context.step())
	{
		clearFramebuffer(f);
		glm::mat4 mod1 = glm::rotate((float)context.getTime(), glm::vec3(0.f, 0.f, 1.f)) *
						 glm::translate(glm::vec3(1, 0, 0)) *
						 glm::scale(glm::vec3((float)context.getTime() *.5f, 1, 0));
		
		glm::mat4 mod2 = glm::rotate((float)context.getTime(), glm::vec3(0.f, 0.f, 1.f)) *
						 glm::scale(glm::vec3((float)context.getTime() *.5f, 1, 0));

		int loc = 0, tex = 0;
		setUniforms(s, loc, tex, (int)context.getKey('R'), (int)context.getKey('G'), (int)context.getKey('B'),
								//glm::vec4(1,1,1,1), glm::vec4(1,0,1,1), glm::vec4(.5,.5,.5,.5),
								mod2*mod1);
		s0_draw(f, s, g);

		loc = 0, tex = 0;
		setUniforms(s, loc, tex, (int)context.getKey('R'), (int)context.getKey('G'), (int)context.getKey('B'),
			mod1);

		s0_draw(f, s, g);
	}
	freeTexture(t_magyel);
	freeTexture(t_mask);
	freeGeometry(g);
	freeShader(s);
	context.term();
	return 0;
}