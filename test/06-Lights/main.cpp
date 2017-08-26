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
	Shader lambert = loadShader("../../resources/shaders/lambert.vert",
								"../../resources/shaders/lambert.frag");

	Texture soil_diffuse = loadTexture("../../resources/textures/sand.jpg");

	Framebuffer screen = { 0, 800, 600 };

	glm::vec3 target_pos = glm::vec3(0, 0, 0);

	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 3, -4), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(45.f, 800.f / 600.f, .01f, 100.f);


	glm::vec4 lightColor = glm::vec4(1, .5, 1, 1);
	glm::vec3 lightPos = glm::vec3(3, 2, 3);
	float lightIntensity = 8.0f;

	while (context.step())
	{

		float time = (float)context.getTime();

		glm::mat4 go_model = glm::rotate(time, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 modq = glm::rotate(glm::radians(90.f), glm::vec3(-1, 0, 0)) * 
						 glm::scale(glm::vec3(5, 5, 1));

		setFlags(RenderFlag::DEPTH);
		clearFramebuffer(screen);

		int loc = 0, tslot = 0;
		setUniforms(lambert, loc, tslot, 
					cam_proj, cam_view, go_model, ss_diffuse,
											lightPos, lightColor);

		loc = 0, tslot = 0;
		setUniforms(lambert, loc, tslot, 
					cam_proj, cam_view, modq, soil_diffuse,
											lightPos, lightColor);

		s0_draw(screen, lambert, ss_geo);
	}

	context.term();

	return 0;
}