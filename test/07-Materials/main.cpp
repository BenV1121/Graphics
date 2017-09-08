#include "graphics\Context.h"
#include "graphics\draw.h"
#include "graphics\Load.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "glm\ext.hpp"
#include "graphics\GameObjects.h"

int main()
{
	Context context;
	context.init(1280, 720);

	Vertex vquad[] = {
		{ { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },
		{ {  1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },
		{ {  1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },
		{ { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } }
	};

	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);
	Geometry quad = makeGeometry(vquad, 4, quadidx, 6);

	Shader standard = loadShader("../../resources/shaders/standard.vert",
								 "../../resources/shaders/standard.frag");

	Framebuffer screen = { 0, 1280, 720 };


	////////////////////////
	/// Model Data
	SpecGloss ss;
	ss.geo = loadGeometry("../../resources/models/soulspear.obj");
	ss.model;

	ss.normal   = loadTexture("../../resources/textures/soulspear_normal.tga");
	ss.diffuse  = loadTexture("../../resources/textures/soulspear_diffuse.tga");
	ss.specular = loadTexture("../../resources/textures/soulspear_specular.tga");
	ss.gloss = 4.0f;

	SpecGloss ss2 = ss;
	ss2.model = glm::rotate(90.f, glm::vec3(0, 0, 1));

	//////////////////////////
	// Camera Data
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 2, 3),
									 glm::vec3(0, 2, 0),
									 glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(45.f, 1280.f / 720.f, .01f, 100.f);

	//////////////////////////
	// Light
	StandardLight l;
	l.dir = glm::normalize(glm::vec3(-2, -1, 1));
	l.color = glm::vec4(1.0, .5, .9, 1);
	l.intensity = 5.0;
	l.ambient = glm::vec4(.2, .5, .1, 1);
	l.type = 0;

	Framebuffer fBuffer = makeFramebuffer(1280, 720, 4, true, 3, 1);

	Shader fsq_shader = loadShader("../../resources/shaders/quad.vert",
								   "../../resources/shaders/quad.frag");
	while (context.step())
	{
		float time = context.getTime();
		ss.model = glm::rotate(time, glm::vec3(0, 1, 0));

		/////////////////////////////////////////////
		////////////// Standard
		clearFramebuffer(fBuffer);
		setFlags(RenderFlag::DEPTH);

		int loc = 0, slot = 0;
		setUniforms(standard, loc, slot,
			cam_proj, cam_view, 			
			ss.model, ss.diffuse, ss.normal, ss.specular, ss.gloss,
			l.dir, l.color, l.intensity, l.ambient, l.type);

		s0_draw(fBuffer, standard, ss.geo);

		loc = 0, slot = 0;
		setUniforms(standard, loc, slot,
			cam_proj, cam_view,
			ss2.model, ss2.diffuse, ss2.normal, ss2.specular, ss2.gloss,
			l.dir, l.color, l.intensity, l.ambient, l.type);

		s0_draw(fBuffer, standard, ss2.geo);

		//////////////////////////////////////////////
		/////////////////// FSQ
		clearFramebuffer(screen);
		setFlags(RenderFlag::DEPTH);

		loc = 0, slot = 0;
		setUniforms(fsq_shader, loc, slot, fBuffer.targets[1], fBuffer.targets[1], 
															   fBuffer.targets[2], 
															   fBuffer.targets[3]);

		s0_draw(screen, fsq_shader, quad);
	}
	context.term();
	return 0;
}