#pragma once

#include <GL/glew.h>

class VertexBuffer {
public:
    VertexBuffer(GLenum type = GL_ARRAY_BUFFER);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    void setData(const void* data, unsigned int size);

    unsigned int getID() const { return m_bufferID; }

private:
    unsigned int m_bufferID;
    GLenum m_type;
};
