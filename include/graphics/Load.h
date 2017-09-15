#pragma once

#include "RenderObjects.h"

Texture loadTexture(const char *path);

Shader loadShader(const char *vpath, const char *fpath);

Shader loadGeoShader(const char *vpath, const char *gpath);

Geometry loadGeometry(const char *path);