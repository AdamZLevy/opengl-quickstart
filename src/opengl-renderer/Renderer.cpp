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

void Renderer::drawArrays(const VertexArray &va, const Shader &shader) const {
    shader.bind();
    va.bind();
    glCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

void Renderer::clear() const {
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
