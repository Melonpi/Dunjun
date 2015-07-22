#ifndef DUNJUN_CORE_OPENGL_HPP
#define DUNJUN_CORE_OPENGL_HPP

// NOTE(bill): This should be defined when compiling but just to make sure
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/GL.h>
#else
#include <GL/gl.h>
#endif

#endif
