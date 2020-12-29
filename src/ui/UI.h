
#ifndef OPENGL_QUICKSTART_UI_H
#define OPENGL_QUICKSTART_UI_H


#include <glfw3.h>
#include <vector>
#include "Widget.h"
#include "widgets/Info.h"

class UI {
private:
    std::vector<Widget*> _widgets;
public:
    UI();
    virtual ~UI();

    void render(GLFWwindow* window);
};


#endif //OPENGL_QUICKSTART_UI_H
