#ifndef OPENGL_QUICKSTART_VERTEXARRAY_H
#define OPENGL_QUICKSTART_VERTEXARRAY_H


#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {

private:
    unsigned int _rendererID{};
    unsigned int _index = 0;
public:
    VertexArray();

    ~VertexArray();

//    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void addBuffer(const VertexBuffer &vb, unsigned int size);
    void bind() const;
    void unbind() const;

};


#endif //OPENGL_QUICKSTART_VERTEXARRAY_H
