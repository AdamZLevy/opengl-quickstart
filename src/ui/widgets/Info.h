#ifndef OPENGL_QUICKSTART_INFO_H
#define OPENGL_QUICKSTART_INFO_H


#include "../Widget.h"

class Info: public Widget {
public:
    void render(GLFWwindow* window) const override;
};


#endif //OPENGL_QUICKSTART_INFO_H
