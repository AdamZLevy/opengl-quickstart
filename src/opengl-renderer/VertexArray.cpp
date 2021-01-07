//
// Created by Adam on 10/24/2020.
//

#include "Renderer.h"
#include "VertexArray.h"

VertexArray::VertexArray() {
    glCall(glGenVertexArrays(1, &_rendererID));
}

VertexArray::~VertexArray() {
    glCall(glDeleteVertexArrays(1, &_rendererID));
}

//void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
//    const auto &elements = layout.getElements();
//    unsigned int offset = 0;
//
//    for (unsigned int i = 0; i < elements.size(); i++) {
//        const auto &element = elements[i];
//        glCall(glEnableVertexAttribArray(i));
//        vb.bind();
//        glCall(glVertexAttribPointer(i, element.count, GL_FLOAT, GL_FALSE, 0,
//                                     (const void *) offset));
//        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
//    }
//}

void VertexArray::addBuffer(const VertexBuffer &vb, const unsigned int size) {
    glCall(glEnableVertexAttribArray(this->_index));
    vb.bind();
    glCall(glVertexAttribPointer(this->_index, size, GL_FLOAT, GL_FALSE, 0, (const void *) 0));
    this->_index++;
}

void VertexArray::bind() const {
    glCall(glBindVertexArray(_rendererID));
}

void VertexArray::unbind() const {
    glCall(glBindVertexArray(0));
}
