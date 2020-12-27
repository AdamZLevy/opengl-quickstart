#ifndef OPENGL_QUICKSTART_TEXTURE_H
#define OPENGL_QUICKSTART_TEXTURE_H

#include "Renderer.h"

class Texture {
private:
    unsigned int _rendererId;
    std::string _filePath;
    unsigned char *_localBuffer;
    int _width, _height, _bpp;
public:
    Texture(const std::string &filePath);

    virtual ~Texture();

    void bind(unsigned int slot = 0) const;

    void unbind();

    inline int getWidth() const { return this->_width; }

    inline int getHeight() const { return this->_height; }
};


#endif //OPENGL_QUICKSTART_TEXTURE_H
