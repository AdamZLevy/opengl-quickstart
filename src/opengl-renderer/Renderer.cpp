#include "Renderer.h"
#include <iostream>

void glClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

bool glPrintErrors(const char *function, const char *file, int line) {
    while (auto error = glGetError()) {
        std::cout << "[OpenGL Error] (" << file << " | " << function << " | " << line << ") " << error << std::endl;
        return false;
    }
    return true;
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const {
    shader.bind();
    va.bind();
    ib.bind();
    glCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawArrays(const VertexArray &va, const Shader &shader, const unsigned int size) const {
    shader.bind();
    va.bind();
    glCall(glDrawArrays(GL_TRIANGLES, 0, size));
}

void Renderer::clear() const {
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::bindFrameBuffer(unsigned int frameBufferId) {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
}

unsigned int Renderer::createFrameBuffer() {
    unsigned int framebufferId;
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    return framebufferId;
}

void Renderer::assignTextureToBuffer(unsigned int textureId) {
    GLuint depthRenderBuffer;
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
}
