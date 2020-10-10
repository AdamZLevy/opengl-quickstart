#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
    glCall(glGenBuffers(1, &_rendererId));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererId));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    glCall(glDeleteBuffers(1, &_rendererId));
}

void VertexBuffer::bind() const {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererId));
}

void VertexBuffer::unbind() const {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
