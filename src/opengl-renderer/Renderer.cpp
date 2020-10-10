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