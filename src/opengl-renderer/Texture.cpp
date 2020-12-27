#include "Texture.h"
#include "stb_image.h"

Texture::Texture(const std::string &filePath)
: _rendererId(0), _filePath(filePath), _localBuffer(nullptr), _width(0), _bpp(0) {
    stbi_set_flip_vertically_on_load(1);
    _localBuffer = stbi_load(_filePath.c_str(), &_width, &_height, &_bpp, 4);

    glCall(glGenTextures(1, &_rendererId));
    glCall(glBindTexture(GL_TEXTURE_2D, _rendererId));

    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer));
    this->unbind();

    if (_localBuffer) {
        stbi_image_free(_localBuffer);
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &_rendererId);
}

void Texture::bind(unsigned int slot) const {
    glCall(glActiveTexture(GL_TEXTURE0 + slot));
    glCall(glBindTexture(GL_TEXTURE_2D, _rendererId));
}

void Texture::unbind() {
    glCall(glBindTexture(GL_TEXTURE_2D, 0));
}
