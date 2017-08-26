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
		{ { -1,-1,0,1 },{},{ 0,0 } },
		{ { 1,-1,0,1 },{},{ 1,0 } },
		{ { 1, 1,0,1 },{},{ 1,1 } },
		{ { -1,1,0,1 },{},{ 0,1 } }
	};

	Geometry ss_geo = loadGeometry("../../resources/models/soulspear.obj");
	Texture ss_diffuse = loadTexture("../../resources/textures/soulspear_diffuse.tga");
	Shader mvplite = loadShader("../../resources/shaders/mvplite.vert",
								"../../resources/shaders/mvplite.frag");
	Framebuffer screen = { 0, 800, 800 };

	glm::vec3 target_pos = glm::vec3(0, 0, 0);

	while (context.step())
	{
		clearFramebuffer(screen);

		glm::mat4 cam_view = glm::lookAt(/*glm::vec3(0, 3, -4)*/ target_pos, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		glm::mat4 cam_proj = glm::perspective(45.f, 800.f / 600.f, .01f, 100.f);
		glm::mat4 go_model;

		glm::mat4 go_model2 = glm::translate(glm::vec3(2, 0, 0)) * 
							  glm::rotate((float)context.getTime(), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 go_model3 = glm::translate(glm::vec3(-2, 0, 0)) *
							  glm::rotate((float)context.getTime(), glm::vec3(0.f, -1.f, 0.f));

		target_pos.x += context.getKey('W') | -context.getKey('S');
		target_pos.y += context.getKey('Q') | -context.getKey('E');
		target_pos.z += context.getKey('D') | -context.getKey('A');

		setFlags(RenderFlag::DEPTH);

		int loc = 0, tslot = 0;
		setUniforms(mvplite, loc, tslot, cam_proj, cam_view, go_model, ss_diffuse);
		s0_draw(screen, mvplite, ss_geo);

		loc = 0, tslot = 0;
		setUniforms(mvplite, loc, tslot, cam_proj, cam_view, go_model2, ss_diffuse);
		s0_draw(screen, mvplite, ss_geo);

		loc = 0, tslot = 0;
		setUniforms(mvplite, loc, tslot, cam_proj, cam_view, go_model3, ss_diffuse);
		s0_draw(screen, mvplite, ss_geo);
	}

	context.term();

	return 0;
}