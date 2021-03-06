#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#ifndef OPENGL_QUICKSTART_RENDERER_H
#define OPENGL_QUICKSTART_RENDERER_H

//#define ASSERT(x) if (!(x)) __debugbreak();
#define ASSERT(x) (x);

#ifdef DEBUG
#define glCall(x) glClearErrors(); x; ASSERT(glPrintErrors(#x, __FILE__, __LINE__))
#else
#define glCall(x) x
#endif

void glClearErrors();

bool glPrintErrors(const char *function, const char *file, int line);

class Renderer {
public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void drawArrays(const VertexArray& va, const Shader& shader, unsigned int size) const;
    unsigned int createFrameBuffer();
    void bindFrameBuffer(unsigned int frameBufferId);
    void assignTextureToBuffer(unsigned int textureId);
};

#endif //OPENGL_QUICKSTART_RENDERER_H