#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "src/opengl-renderer/Renderer.h"
#include "src/opengl-renderer/VertexBuffer.h"
#include "src/opengl-renderer/IndexBuffer.h"
#include "src/opengl-renderer/VertexArray.h"
#include "src/opengl-renderer/Shader.h"
#include "src/opengl-renderer/Texture.h"

static void GLAPIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam) {
    printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
           "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
    printf("%s\n", message);
}

int main() {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 1024, "OpenGL Quickstart", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Sync with monitor refresh rate */
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW Init error" << std::endl;
    }

#ifdef DEBUG
    std::cout << "Debug run" << std::endl;
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glMessageCallback, nullptr);
#else
    std::cout << "Release run" << std::endl;
#endif

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] = {
                -0.5f, -0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        glCall(glEnable(GL_BLEND));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.pushFloat(2);
        layout.pushFloat(2);
        va.addBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader vertexShader(GL_VERTEX_SHADER, "shader/BaseVertexShader.glsl");
        vertexShader.bind();
        Shader fragmentShader(GL_FRAGMENT_SHADER, "shader/BaseFragmentShader.glsl");
        fragmentShader.bind();

        Texture texture("resources/img/no_image.png");
        texture.bind();
        fragmentShader.setUniform1i("u_Texture", 0);

        va.unbind();
        vb.unbind();
        vertexShader.unbind();
        fragmentShader.unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */

            renderer.clear();

            renderer.draw(va, ib, fragmentShader);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
