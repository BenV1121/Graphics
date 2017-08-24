#pragma once

#include "RenderObjects.h"

Texture loadTexture(const char *path);

Shader loadShader(const char *vpath, const char *fpath);

Geometry loadGeometry(const char *path);