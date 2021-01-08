#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui_impl_opengl3.h>
#include <fstream>
#include <sstream>
#include <glm.hpp>
#include <ext.hpp>
#include <stb_image.h>

#include "src/opengl-renderer/Renderer.h"
#include "src/opengl-renderer/VertexBuffer.h"
#include "src/opengl-renderer/IndexBuffer.h"
#include "src/opengl-renderer/VertexArray.h"
#include "src/opengl-renderer/Shader.h"
#include "src/opengl-renderer/Texture.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "src/ui/UI.h"
GLFWwindow *window;
static void GLAPIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                         const GLchar *message, const void *userParam) {
    printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
           "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
    printf("%s\n", message);
}

float cx = 4;
bool viewWindowActive = false;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (viewWindowActive) cx += yoffset * 0.05;
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
    // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(1024, 768, "Open GL Quickstart", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMaximizeWindow(window);
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    {
        static const GLfloat g_cube_vertex_buffer_data[] = {
                -1.0f,-1.0f,-1.0f, // triangle 1 : begin
                -1.0f,-1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f, // triangle 1 : end
                1.0f, 1.0f,-1.0f, // triangle 2 : begin
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f, // triangle 2 : end
                1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                1.0f,-1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f,-1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f,-1.0f, 1.0f
        };
        static const GLfloat g_uv_buffer_data[] = {
                0.000059f, 1.0f-0.000004f,
                0.000103f, 1.0f-0.336048f,
                0.335973f, 1.0f-0.335903f,
                1.000023f, 1.0f-0.000013f,
                0.667979f, 1.0f-0.335851f,
                0.999958f, 1.0f-0.336064f,
                0.667979f, 1.0f-0.335851f,
                0.336024f, 1.0f-0.671877f,
                0.667969f, 1.0f-0.671889f,
                1.000023f, 1.0f-0.000013f,
                0.668104f, 1.0f-0.000013f,
                0.667979f, 1.0f-0.335851f,
                0.000059f, 1.0f-0.000004f,
                0.335973f, 1.0f-0.335903f,
                0.336098f, 1.0f-0.000071f,
                0.667979f, 1.0f-0.335851f,
                0.335973f, 1.0f-0.335903f,
                0.336024f, 1.0f-0.671877f,
                1.000004f, 1.0f-0.671847f,
                0.999958f, 1.0f-0.336064f,
                0.667979f, 1.0f-0.335851f,
                0.668104f, 1.0f-0.000013f,
                0.335973f, 1.0f-0.335903f,
                0.667979f, 1.0f-0.335851f,
                0.335973f, 1.0f-0.335903f,
                0.668104f, 1.0f-0.000013f,
                0.336098f, 1.0f-0.000071f,
                0.000103f, 1.0f-0.336048f,
                0.000004f, 1.0f-0.671870f,
                0.336024f, 1.0f-0.671877f,
                0.000103f, 1.0f-0.336048f,
                0.336024f, 1.0f-0.671877f,
                0.335973f, 1.0f-0.335903f,
                0.667969f, 1.0f-0.671889f,
                1.000004f, 1.0f-0.671847f,
                0.667979f, 1.0f-0.335851f
        };

        Renderer renderer;

        Texture renderedTexture;

        unsigned int textureRenderBufferId = renderer.createFrameBuffer();
        renderer.assignTextureToBuffer(renderedTexture.getId());

        // --------------------------------------

        VertexArray va;
        va.bind();

        // This will identify our vertex buffer
        VertexBuffer vb(g_cube_vertex_buffer_data, sizeof(g_cube_vertex_buffer_data));
        va.addBuffer(vb, 3);

        VertexBuffer uv_vb(g_uv_buffer_data, sizeof(g_uv_buffer_data));
        va.addBuffer(uv_vb, 2);

        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        Shader shader("shader/BaseVertexShader.glsl", "shader/BaseFragmentShader.glsl");

        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 model = glm::mat4(1.0f);

        UI ui;

        int width;
        int height;
//        float cx = 4;
        float cy = 3;
        float cz = 3;
        float deltaTime = 1;
        float speed = 0.05;

        Texture texture("resources/img/texture.png");
        texture.bind();

        ImVec2 windowSize(1024, 768);
        while (!glfwWindowShouldClose(window)) {
            glfwGetWindowSize(window, &width, &height);

            renderer.bindFrameBuffer(textureRenderBufferId);
            glViewport(0, 0, windowSize.x, windowSize.y);

            renderer.clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && viewWindowActive) {
                cz += deltaTime * speed;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS  && viewWindowActive) {
                cz -= deltaTime * speed;
            }
            glfwSetScrollCallback(window, scroll_callback);

            // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
            glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                    (float) windowSize.x / (float) windowSize.y,
                                                    0.1f, 100.0f);
            // Or, for an ortho camera :
            //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

            // Camera matrix
            glm::mat4 view = glm::lookAt(
                    glm::vec3(cx,cy,cz), // Camera is at (4,3,3), in World Space
                    glm::vec3(0,0,0), // and looks at the origin
                    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
            );

            // Our ModelViewProjection : multiplication of our 3 matrices
            glm::mat4 mvp = projection * view * model; // Remember, matrix multiplication is the other way around

            shader.setMatrix4fv("MVP", mvp);

            renderer.drawArrays(va, shader, sizeof(g_cube_vertex_buffer_data) / 6 / 2);

            renderer.bindFrameBuffer(0);
            glViewport(0, 0, width, height);

            renderer.clear();

            ImGui::Begin("Camera");
            ImGui::SliderFloat("x", &cx, -100, 100);
            ImGui::SliderFloat("y", &cy, -100, 100);
            ImGui::SliderFloat("z", &cz, -100, 100);
            ImGui::End();

            ImGui::Begin("Texture");
            ImGui::Image((void*) texture.getId(), ImVec2(texture.getWidth(), texture.getHeight()), ImVec2(0, 1), ImVec2(1, 0));
            ImGui::End();

            ImGui::Begin("Render to texture");
            windowSize = ImGui::GetWindowSize();
            ImGui::Image((void*) renderedTexture.getId(), windowSize, ImVec2(0, 1), ImVec2(1, 0));
            viewWindowActive = ImGui::IsWindowHovered();
            ImGui::End();

            ui.render(window);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();

        };
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
