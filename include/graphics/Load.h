#pragma once

#include "RenderObjects.h"

Texture loadTexture(const char *path);

CubeTexture loadCubeMap(const char *path_Xpos, const char *path_Xneg,
						const char *path_Ypos, const char *path_Yneg,
						const char *path_Zpos, const char *path_Zneg);

Shader loadShader(const char *vpath, const char *fpath);

Shader loadGeoShader(const char *vpath, const char *gpath);

Geometry loadGeometry(const char *path);