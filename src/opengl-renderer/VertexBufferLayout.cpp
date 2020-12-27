#include <iostream>
#include "VertexBufferLayout.h"

void VertexBufferLayout::pushFloat(unsigned int count) {
    _elements.push_back({GL_FLOAT, count, GL_FALSE});
    _stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

void VertexBufferLayout::pushUInt(unsigned int count) {
    _elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    _stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

void VertexBufferLayout::pushUChar(unsigned int count) {
    _elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    _stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}