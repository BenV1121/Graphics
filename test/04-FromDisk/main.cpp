#include "graphics\draw.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Context.h"
#include "graphics\Load.h"

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

	Framebuffer screen = { 0, 800, 800 };

	while (context.step())
	{
		clearFramebuffer(screen);

		int loc = 0, tslot = 0;
		setUniforms(s, loc, tslot, tex);

		s0_draw(screen, s, quad);
	}

	context.term();

	return 0;
};