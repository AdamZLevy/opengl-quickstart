#ifndef OPENGL_QUICKSTART_VERTEXBUFFERLAYOUT_H
#define OPENGL_QUICKSTART_VERTEXBUFFERLAYOUT_H


#include <vector>
#include "GL/gl.h"

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(GLfloat);
            case GL_UNSIGNED_INT:
                return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:
                return sizeof(GLbyte);
            default:
                return 0;
        }
    }
};

class VertexBufferLayout {
private:
    unsigned int _stride;
    std::vector<VertexBufferElement> _elements;
public:
    VertexBufferLayout()
            : _stride(0) {}

    void pushFloat(unsigned int count);
    void pushUInt(unsigned int count);
    void pushUChar(unsigned int count);

    inline std::vector<VertexBufferElement> getElements() const { return _elements; }

    inline unsigned int getStride() const { return _stride; }

};


#endif //OPENGL_QUICKSTART_VERTEXBUFFERLAYOUT_H
