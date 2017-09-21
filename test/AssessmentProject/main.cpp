#include "graphics\GameObjects.h"
#include "graphics\Context.h"
#include "graphics\draw.h"
#include "graphics\Load.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "glm\ext.hpp"

void main()
{
	Context context;
	context.init(1280, 720);

	Vertex vquad[] =
	{
		{ { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },
		{ {  1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },
		{ {  1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },
		{ { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } }
	};
	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	solveTangents(vquad, 4, quadidx, 6);
	Geometry quad = makeGeometry(vquad, 4, quadidx, 6);

	Geometry skycube = loadGeometry("../../resources/models/cube.obj");
	glm::mat4 model;
	
	SpecGloss objects[4];

	// Floor
	objects[0].geo = quad;
	objects[0].model = glm::rotate(glm::radians(90.f), glm::vec3(-1, 0, 0))
		*glm::scale(glm::vec3(5, 5, 1));
	objects[0].diffuse = loadTexture("../../resources/textures/sand.jpg");
	objects[0].gloss = 4;

	CubeTexture lcb = loadCubeMap("../../resources/textures/skybox/dejavu_bk.tga", "../../resources/textures/skybox/dejavu_ft.tga",
								  "../../resources/textures/skybox/dejavu_dn.tga", "../../resources/textures/skybox/dejavu_up.tga",
								  "../../resources/textures/skybox/dejavu_lf.tga", "../../resources/textures/skybox/dejavu_rt.tga");

	// Camera
	Camera cam;
	cam.view = glm::lookAt(glm::vec3(0, 1, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	cam.proj = glm::perspective(45.f, 1280.f / 720.f, 1.f, 1000.f);

	DirectionalLight dlights[1];
		
	dlights[0].range = 10;
	dlights[0].intensity = 1;
	dlights[0].color = glm::vec4(1, 1, 0, 1);
	dlights[0].direction = glm::vec3(1, -1, 1);

	Shader gpass = loadShader("../../resources/shaders/Cubemap.vert", "../../resources/shaders/Cubemap.frag");
	Framebuffer screen = { 0,1280,720 };
	Framebuffer gbuffer = makeFramebuffer(1280, 720,  4, true,  2, 2);
	Framebuffer lbuffer = makeFramebuffer(1280, 720,  4, false, 2, 0);
	Framebuffer sbuffer = makeFramebuffer(1024, 1024, 0, true,   0, 0);

	int loc = 0, slot = 0;
	while (context.step())
	{
		clearFramebuffer(screen);
		setFlags(RenderFlag::DEPTH);

		glm::mat4 model = glm::rotate((float)context.getTime(), glm::vec3(0.f, 1.f, 0.f)) * glm::scale(glm::vec3(11, 11, 11));

		loc = slot = 0;
		setUniforms(gpass, loc, slot, cam, model, lcb);
		s0_draw(screen, gpass, skycube);

		//loc = slot = 0;
		//setUniforms(geoShade, loc, slot, cam, objects[0], dlights[0]);
		//s0_draw(screen, geoShade, objects[0].geo);
	}
	context.term();
}