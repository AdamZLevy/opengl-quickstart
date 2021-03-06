#ifndef OPENGL_QUICKSTART_VERTEXBUFFER_H
#define OPENGL_QUICKSTART_VERTEXBUFFER_H
#include <GL/glew.h>

class VertexBuffer {

public:
    unsigned int _rendererId;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

};


#endif //OPENGL_QUICKSTART_VERTEXBUFFER_H
