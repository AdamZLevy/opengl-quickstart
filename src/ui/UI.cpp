#include "UI.h"
#include "imgui.h"

UI::UI() {
    auto info = new Info();
    _widgets.push_back(info);
}

UI::~UI() {
    for (const auto &item : _widgets) {
        delete item;
    }
}

void UI::render(GLFWwindow* window) {
    ImGui::ShowDemoWindow();
    for (const auto &item : _widgets) {
        item->render(window);
    }
}
