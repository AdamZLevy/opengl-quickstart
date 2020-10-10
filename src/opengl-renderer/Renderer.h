#include <GL/glew.h>

#ifndef OPENGL_QUICKSTART_RENDERER_H
#define OPENGL_QUICKSTART_RENDERER_H

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
#define glCall(x) glClearErrors(); x; ASSERT(glPrintErrors(#x, __FILE__, __LINE__))
#else
#define glCall(x) x
#endif

void glClearErrors();

bool glPrintErrors(const char *function, const char *file, int line);

class Renderer {
};

#endif //OPENGL_QUICKSTART_RENDERER_H