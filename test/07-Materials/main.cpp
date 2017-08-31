#include "graphics\draw.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\Context.h"
#include "graphics\Load.h"
#include "glm\ext.hpp"

int main()
{
	Context context;
	context.init();

	Vertex vquad[] =
	{
		{ { -1,-1,0,1 },{},{ 0,0 } },
		{ { 1,-1,0,1 },{},{ 1,0 } },
		{ { 1, 1,0,1 },{},{ 1,1 } },
		{ { -1,1,0,1 },{},{ 0,1 } }
	};

	unsigned quadidxs[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidxs, 6);
	Geometry quad = makeGeometry(vquad, 4, quadidxs, 6);

	Texture floor_normal  = loadTexture("../../resources/textures/floor_normal.jpg");
	Texture floor_diffuse = loadTexture("../../resources/textures/ken2.png");

	Shader standard = loadShader("../../resources/shaders/standard.vert",
								 "../../resources/shaders/standard.frag");

	Framebuffer screen = { 0,800,600 };

	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 3,-4),
									 glm::vec3(0, 1, 0),
									 glm::vec3(0, 1, 0));

	glm::mat4 cam_proj = glm::perspective(45.f, 800.f / 600.f, .01f, 100.f);

	glm::mat4 go_model;

	glm::vec3 light_direction = glm::normalize(glm::vec3(1, -1, 1));

	while (context.step())
	{
		float time = context.getTime();

		go_model = glm::rotate(time, glm::vec3(0,-1,0)) *
				   glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)) *
			       glm::scale(glm::vec3(2,2,1));

		clearFramebuffer(screen);
		setFlags(RenderFlag::DEPTH);

		// setuniform(standard, 0, cam_proj);
		// setuniform(standard, 1, cam_view);
		// setuniform(standard, 2, go_model);
		// setuniform(standard, 3, floor_normal, 0);

		int loc = 0, slot = 0;
		setUniforms(standard, loc, slot,
							cam_proj, cam_view,
							go_model, floor_normal, floor_diffuse,
							light_direction);

		s0_draw(screen, standard, quad);
	}

	context.term();

	return 0;
}