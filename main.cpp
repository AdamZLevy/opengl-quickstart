#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui_impl_opengl3.h>
#include <fstream>
#include <sstream>

#include "src/opengl-renderer/Renderer.h"
#include "src/opengl-renderer/VertexBuffer.h"
#include "src/opengl-renderer/IndexBuffer.h"
#include "src/opengl-renderer/VertexArray.h"
#include "src/opengl-renderer/Shader.h"
#include "src/opengl-renderer/Texture.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "src/ui/UI.h"

static void GLAPIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                         const GLchar *message, const void *userParam) {
    printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
           "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
    printf("%s\n", message);
}

int main() {
    glewExperimental = true; // Needed for core profile
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

// Open a window and create its OpenGL context
    GLFWwindow *window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    {
        float g_vertex_buffer_data[9] = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
        };

        // -----------------------------------------------------------------------------------------------------------------
        VertexArray va;
        va.bind();

        // This will identify our vertex buffer
        VertexBuffer vb(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vb._rendererId);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );
        // -----------------------------------------------------------------------------------------------------------------

        Renderer renderer;
        Shader shader("shader/BaseVertexShader.glsl", "shader/BaseFragmentShader.glsl");

        do {
            // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Draw nothing, see you in tutorial 2 !

            renderer.drawArrays(va, shader);

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();

        } // Check if the ESC key was pressed or the window was closed
        while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
               glfwWindowShouldClose(window) == 0);
    }
}

