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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
//    glEnable(GL_TEXTURE_2D);
//    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//    glCall(glEnable(GL_BLEND));

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    {
//        float g_triangle_vertex_buffer_data[] = {
//                -1.0f, -1.0f, 0.0f,
//                1.0f, -1.0f, 0.0f,
//                0.0f, 1.0f, 0.0f,
//        };

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
//        static const GLfloat g_color_buffer_data[] = {
//                0.583f,  0.771f,  0.014f,
//                0.609f,  0.115f,  0.436f,
//                0.327f,  0.483f,  0.844f,
//                0.822f,  0.569f,  0.201f,
//                0.435f,  0.602f,  0.223f,
//                0.310f,  0.747f,  0.185f,
//                0.597f,  0.770f,  0.761f,
//                0.559f,  0.436f,  0.730f,
//                0.359f,  0.583f,  0.152f,
//                0.483f,  0.596f,  0.789f,
//                0.559f,  0.861f,  0.639f,
//                0.195f,  0.548f,  0.859f,
//                0.014f,  0.184f,  0.576f,
//                0.771f,  0.328f,  0.970f,
//                0.406f,  0.615f,  0.116f,
//                0.676f,  0.977f,  0.133f,
//                0.971f,  0.572f,  0.833f,
//                0.140f,  0.616f,  0.489f,
//                0.997f,  0.513f,  0.064f,
//                0.945f,  0.719f,  0.592f,
//                0.543f,  0.021f,  0.978f,
//                0.279f,  0.317f,  0.505f,
//                0.167f,  0.620f,  0.077f,
//                0.347f,  0.857f,  0.137f,
//                0.055f,  0.953f,  0.042f,
//                0.714f,  0.505f,  0.345f,
//                0.783f,  0.290f,  0.734f,
//                0.722f,  0.645f,  0.174f,
//                0.302f,  0.455f,  0.848f,
//                0.225f,  0.587f,  0.040f,
//                0.517f,  0.713f,  0.338f,
//                0.053f,  0.959f,  0.120f,
//                0.393f,  0.621f,  0.362f,
//                0.673f,  0.211f,  0.457f,
//                0.820f,  0.883f,  0.371f,
//                0.982f,  0.099f,  0.879f
//        };
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

        VertexArray va;
        va.bind();

        // This will identify our vertex buffer
        VertexBuffer vb(g_cube_vertex_buffer_data, sizeof(g_cube_vertex_buffer_data));
        va.addBuffer(vb, 3);

//        VertexBuffer color_vb(g_color_buffer_data, sizeof(g_color_buffer_data));
//        va.addBuffer(color_vb, 3);

        VertexBuffer uv_vb(g_uv_buffer_data, sizeof(g_uv_buffer_data));
        va.addBuffer(uv_vb, 2);

        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        Renderer renderer;
        Shader shader("shader/BaseVertexShader.glsl", "shader/BaseFragmentShader.glsl");

        // Or, for an ortho camera :
        //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 model = glm::mat4(1.0f);

        UI ui;

        int width;
        int height;
        float cx = 4;
        float cy = 3;
        float cz = 3;

        Texture texture("resources/img/texture.png");
        texture.bind();

        while (!glfwWindowShouldClose(window)) {
            glfwGetWindowSize(window, &width, &height);

            renderer.clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glViewport(0, 0, width, height);

            ImGui::Begin("Camera");
            ImGui::SliderFloat("x", &cx, -100, 100);
            ImGui::SliderFloat("y", &cy, -100, 100);
            ImGui::SliderFloat("z", &cz, -100, 100);
            ImGui::End();

            // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
            glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                    (float) width / (float) height,
                                                    0.1f, 100.0f);

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

