#include "..\include\graphics\GameObjects.h"
#include "graphics\draw.h"

void __internal::t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Camera &val)
{
	t_setUniform(s, loc_io, tex_io, val.proj); // 0
	t_setUniform(s, loc_io, tex_io, val.view); // 1

}

void __internal::t_setUniform(const Shader &s, int &loc_io, int &tex_io, const SpecGloss &val)
{
	t_setUniform(s, loc_io, tex_io, val.model);		// 0 matrix
	
	t_setUniform(s, loc_io, tex_io, val.diffuse);	// 1 texture
	t_setUniform(s, loc_io, tex_io, val.specular);	// 2 texture
	t_setUniform(s, loc_io, tex_io, val.normal);	// 3 texture
	t_setUniform(s, loc_io, tex_io, val.gloss);		// 4 float
}

void __internal::t_setUniform(const Shader &s, int &loc_io, int &tex_io, const DirectionalLight &val)
{
	t_setUniform(s, loc_io, tex_io, val.getProj());
	t_setUniform(s, loc_io, tex_io, val.getView());
	t_setUniform(s, loc_io, tex_io, val.color);
	t_setUniform(s, loc_io, tex_io, val.intensity);
}
