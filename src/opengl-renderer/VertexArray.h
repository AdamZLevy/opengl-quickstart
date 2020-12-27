#ifndef OPENGL_QUICKSTART_VERTEXARRAY_H
#define OPENGL_QUICKSTART_VERTEXARRAY_H


#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {

private:
    unsigned int _rendererID{};
public:
    VertexArray();

    ~VertexArray();

    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void bind() const;
    void unbind() const;

};


#endif //OPENGL_QUICKSTART_VERTEXARRAY_H
