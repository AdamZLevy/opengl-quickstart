#ifndef OPENGL_QUICKSTART_WIDGET_H
#define OPENGL_QUICKSTART_WIDGET_H


#include <glfw3.h>

class Widget {
public:
    virtual void render(GLFWwindow* window) const {};
};


#endif //OPENGL_QUICKSTART_WIDGET_H
