#pragma once

#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "RenderObjects.h"

struct Camera
{
	glm::mat4 proj;
	glm::mat4 view;
};

struct SpecGloss
{
	Geometry geo;

	glm::mat4 model;
	Texture diffuse;
	Texture specular;
	Texture normal;
	float gloss;
};

struct StandardLight
{
	glm::vec3 dir;
	glm::vec4 color;
	float intensity;
	glm::vec4 ambient;
	int type;
};

struct DirectionalLight
{
	glm::vec3 target; // for shadow mapping
	float range;	  // for shadow mapping

	glm::vec3 direction;

	// 0
	glm::mat4 getProj() const
	{
		return glm::ortho<float>(-range, range, -range, range, -range, range);
	}

	// 1
	glm::mat4 getView() const
	{
		return glm::lookAt(-direction + target, target, glm::vec3(0, 1, 0));
	}

	glm::vec4 color; // 2
	float intensity; // 3
};

struct Particle
{
	Particle()
		: m_Position(0)
		, m_Velocity(0)
		, m_Color(0)
		, m_fRotate(0)
		, m_fAge(0)
		, m_fLifeTime(0)
	{}

	glm::vec3 m_Position;
	glm::vec3 m_Velocity;
	glm::vec4 m_Color;
	float m_fRotate;
	float m_fSize;
	float m_fAge;
	float m_fLifeTime;
};

namespace __internal
{
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Camera &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const SpecGloss &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const DirectionalLight &val);
}