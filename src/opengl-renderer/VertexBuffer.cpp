#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
    glCall(glGenBuffers(1, &this->_rendererId));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, this->_rendererId));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    glCall(glDeleteBuffers(1, &this->_rendererId));
}

void VertexBuffer::bind() {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, this->_rendererId));
}

void VertexBuffer::unbind() {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
