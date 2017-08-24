#include "graphics\draw.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Context.h"

int main()
{
	Context context;
	context.init(800, 800);
	Vertex verts[4] = 
  { { { .5,  .5,  0, 1 },{ 1,1,0,1 }, {0,0} },   //0
	{ {-.5,  .5,  0, 1 },{ 0,1,1,1 }, {1,0} },   //1
	{ {-.5, -.5,  0, 1 },{ 1,0,1,1 }, {1,1} },   //2
	{ { .5, -.5,  0, 1 },{ 1,0,0,1 }, {0,1} } }; //3

	unsigned idxs[6] = { 1,2,3, 0,3,1 };

	Geometry g = makeGeometry(verts, 4, idxs, 6);

	const char* vsource = // once per vertex
		"#version 450\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec4 color;\n"
		"layout(location = 0) uniform float time;\n"
		"layout(location = 2) uniform float xpos;\n"
		"layout(location = 3) uniform float ypos;\n"
		"out vec4 vColor;\n"
		"out vec2 vUV;\n"
		"void main ()\n"
		"{\n"
		"gl_Position = position;\n"
		//"gl_Position.x += sin(time*40)/2.0;\n"
		//"gl_Position.y += sin(time+position.y)/5.0;\n"
		"vColor = color;\n"
		"vUV = position.xy;\n"
		"}\n";

	const char* fsource = // once per pixel
		"#version 450\n"
		"out vec4 outColor;\n"
		"layout(location = 0) uniform float time;\n"
		//"layout(location = 1) uniform int tog;\n"
		"layout(location = 4) uniform sampler2D map;\n"
		"layout(location = 5) uniform sampler2D mask;\n"
		"in vec4 vColor;\n"
		"in vec2  vUV;\n"
		"void main ()\n"
		"{\n"
		"vec2 uv = vUV;\n"
		//"uv.x += sin(time + iv.y);\n"
		//"outColor = texture (map,uv);\n"
		//"outColor = texture(map, vUV);\n"
		//"outColor.a = 1;\n"
		//"outCOlor = vec4(vUV,0,1);\n"
		/*"if(tog == 1)\n"
		"outColor = 1.0 - vColor;\n"
		"else outColor = vColor;\n"*/
		"outColor.r = 2 * sin((gl_FragCoord.y * 3) + 2);\n"
		"}\n";

	Shader s = makeShader(vsource, fsource);

	Framebuffer f = { 0, 800, 800 };

	// RGB texture (3 channels), 2x1
	unsigned char pixels[] = {255, 0, 255,
							  255, 255, 0};
	Texture t_mask = makeTexture(2, 1, 3, pixels);
	Texture t_magyel = makeTexture(2, .2f, 4, pixels);

	float xpos = 0, ypos = 0;
	glm::vec2 pos;
	float prevTime = 0;
	float speed = 1.2f;
	while (context.step())
	{
		float ct = context.getTime();
		float dt = ct - prevTime;
		prevTime = ct;

		glm::vec2 vel = { 0,0 };
		pos.y += context.getKey('W');
		pos.x -= context.getKey('A');
		pos.y -= context.getKey('S');
		pos.x += context.getKey('D');

		if (glm::length(vel) > 0)
			pos += glm::normalize(vel) *dt * speed;

		clearFramebuffer(f);
		
		setUniform(s, 0, (float) context.getTime());
		setUniform(s, 1, context.getKey(' '));
		setUniform(s, 2, pos.x);
		setUniform(s, 3, pos.y);

		setUniform(s, 4, t_magyel, 0);
		setUniform(s, 5, t_mask,   1);

		//setUniform(s, time, pos, t_magyel, t_mask, tex1, tex2);

		s0_draw(f, s, g);
	}

	freeGeometry(g);
	freeShader(s);
	freeTexture(t_magyel);
	freeTexture(t_mask);

	context.term();
	return 0;
}