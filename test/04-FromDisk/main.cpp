#include "graphics\draw.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Context.h"
#include "graphics\Load.h"

#include "glm\ext.hpp"

int main()
{
	Context context;
	context.init(800, 800);

	Vertex vquad[] = 
	{
		{{-1,-1,0,1},{},{0,0}},
		{{ 1,-1,0,1},{},{1,0}},
		{{ 1, 1,0,1},{},{1,1}},
		{{ -1,1,0,1},{},{0,1}} 
	};

	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	Geometry quad = makeGeometry(vquad, 4, quadidx, 6);

	Geometry cube = loadGeometry("../../resources/models/cube.obj");

	Texture tex = loadTexture("../../resources/textures/red.png");

	//Shader s = makeShader(vsource, fsource);
	Shader s = loadShader("../../resources/shaders/test.vert", 
						  "../../resources/shaders/test.frag");

	Shader scube = loadShader("../../resources/shaders/cube.vert",
							  "../../resources/shaders/cube.frag");

	Framebuffer screen = { 0, 800, 800 };

	float x = 0, y = 0;
	while (context.step())
	{
		clearFramebuffer(screen);
		float time = context.getTime();

		int frame = 3;
		frame += context.getKey('A') * 3;
		frame += context.getKey('D') * 2;
		frame += context.getKey('W') * 1;
		frame % 4;

		x += context.getKey('D') * .016;
		y += context.getKey('W') * .016;
		x -= context.getKey('A') * .016;
		y -= context.getKey('S') * .016;

		int loc = 0, tslot = 0;
		setUniforms(s, loc, tslot, tex, (int)(time*3)%4+frame*4,4,4,x,y);
		s0_draw(screen, s, quad);

		glm::mat4 mod_cube = glm::rotate(time, glm::vec3(1, 1, 1));

		setFlags(RenderFlag::DEPTH);

		loc = 0, tslot = 0;
		setUniforms(scube, loc, tslot, mod_cube);
		s0_draw(screen, scube, cube);
	}

	context.term();

	return 0;
};