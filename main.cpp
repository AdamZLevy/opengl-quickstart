#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "src/opengl-renderer/Renderer.h"
#include "src/opengl-renderer/VertexBuffer.h"
#include "src/opengl-renderer/IndexBuffer.h"

static void GLAPIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam) {
    printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
           "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
    printf("%s\n", message);
}

static std::string getShader(const std::string &path) {
    std::ifstream fileStream(path);
    std::stringstream shader;
    if (fileStream.is_open()) {
        shader << fileStream.rdbuf();
        fileStream.close();
    }
    return shader.str();
}

static std::string getShaderTypeStr(const unsigned int type) {
    switch (type) {
        case GL_FRAGMENT_SHADER:
            return "Fragment";
        case GL_VERTEX_SHADER:
            return "Vertex";
        default:
            return std::to_string(type);
    }
}

static unsigned int compileShader(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();

    glCall(glShaderSource(id, 1, &src, nullptr));
    glCall(glCompileShader(id));

    int result;
    glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length];
        glCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << getShaderTypeStr(type) << " shader" << std::endl;
        std::cout << message << std::endl;
        glCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    glCall(unsigned int program = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glCall(glAttachShader(program, vs));
    glCall(glAttachShader(program, fs));
    glCall(glLinkProgram(program));
    glCall(glValidateProgram(program));

    glDeleteProgram(vs);
    glDeleteProgram(fs);

    return program;
}

int main() {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

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

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Init error" << std::endl;

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
                -0.5f, -0.5f,
                0.5f, 0.5f,
                0.5f, -0.5f,
                -0.5f, 0.5f
        };

        unsigned int indices[] = {
                0, 3, 1,
                2, 3, 1
        };

        unsigned int vao;
        glCall(glGenVertexArrays(1, &vao));
        glCall(glBindVertexArray(vao));

        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        glCall(glEnableVertexAttribArray(0));
        glCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

        IndexBuffer ib(indices, 6);

        unsigned int shader = createShader(getShader("shader/BaseVertexShader.glsl"),
                                           getShader("shader/BaseFragmentShader.glsl"));

        int location = glGetUniformLocation(shader, "u_Color");
        ASSERT(location != -1);
        glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f);

        glCall(glBindVertexArray(0));
        glCall(glUseProgram(0));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float increment = 0.01f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glCall(glClear(GL_COLOR_BUFFER_BIT));

            if (r >= 1.0f)
                increment = -0.01f;
            else if (r <= 0)
                increment = 0.01f;

            r += increment;

            glUseProgram(shader);
            glUniform4f(location, r, 0.0f, 0.0f, 1.0f);

            glCall(glBindVertexArray(vao));
            ib.bind();

            glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            /* Swap front and back buffers */
            glCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            glCall(glfwPollEvents());
        }
    }

    glfwTerminate();
    return 0;
}
