#include <imgui.h>
#include "Info.h"

void Info::render(GLFWwindow* window) const {
    int width;
    glfwGetWindowSize(window, &width, nullptr);
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoInputs;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDecoration;
    ImGui::Begin("Info", NULL, window_flags);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    ImGui::SetWindowPos(ImVec2(width - ImGui::GetWindowWidth(), 0));
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
