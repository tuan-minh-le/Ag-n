#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(GLenum type)
    : m_type(type)
{
    glGenBuffers(1, &m_bufferID);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_bufferID);
}

void VertexBuffer::bind() const {
    glBindBuffer(m_type, m_bufferID);
}

void VertexBuffer::unbind() const {
    glBindBuffer(m_type, 0);
}

void VertexBuffer::setData(const void* data, unsigned int size) {
    bind();
    glBufferData(m_type, size, data, GL_STATIC_DRAW);
}
