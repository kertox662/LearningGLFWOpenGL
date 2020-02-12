#ifndef VERTEXBUFFER_H //VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "GLCommon.hpp"

class VertexBuffer{
private:
    unsigned int m_RendererID;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

};

#endif //VERTEXBUFFER_H