#pragma once

struct Geometry;
struct Shader;
struct Framebuffer;
struct Texture;

void s0_draw(const Framebuffer &f,
			 const Shader &s,
			 const Geometry &g);


void clearFramebuffer(const Framebuffer &f);

void setUniform(const Shader &s, int location, float value);
void setUniform(const Shader &s, int location, int value);

void setUniform(const Shader &s, int location, const Texture &value, unsigned slot);
